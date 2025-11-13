// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "os/File.hpp"

#include "error.hpp"
#include "error/FileError.hpp"
#include "error/Generic.hpp"
#include "macros.hpp"
#include <limits>

extern "C" {
#include "debug.h"
}

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#define access _access
#ifndef R_OK
#define R_OK 0b0100
#endif
#ifndef W_OK
#define W_OK 0b0010
#endif
#ifndef X_OK
#define X_OK 0b0001
#endif
#ifndef F_OK
#define F_OK 0b0000
#endif
#else
#include <unistd.h>
#endif

extern "C" {
#include <sys/stat.h>
}

#define FILE_ERROR(_msg)                                                       \
  {                                                                            \
    std::ostringstream _err_msg;                                               \
    _err_msg << _msg;                                                          \
    throw fag::Error::FileError(_err_msg.str().c_str());                       \
  }

namespace fag {
namespace OS {

FileBuffer::FileBuffer(const std::string &file_path, FileAccessMode mode)
    : accessMode(mode), m_FilePath(file_path), m_IndexInFile(0) {
  struct stat filestat{};
  if (0 != stat(file_path.c_str(), &filestat)) {
    int err_number = errno;
    FILE_ERROR("could not stat file '" << file_path << "'. "
                                       << strerror(err_number));
  }

  if (static_cast<uint8_t>(mode) | static_cast<uint8_t>(FileAccessMode::Read) &&
      0 != access(file_path.c_str(), R_OK))
    FILE_ERROR("no read permission for file '" << file_path << "'");

  if (static_cast<uint8_t>(mode) |
          static_cast<uint8_t>(FileAccessMode::Write) &&
      0 != access(file_path.c_str(), W_OK))
    FILE_ERROR("no write permission for file '" << file_path << "'");

  m_Size = filestat.st_size;
  int64_t remaining_size = m_Size;

  FILE *fp = fopen(file_path.c_str(), "rb");
  if (nullptr == fp) {
    int err_number = errno;
    FILE_ERROR("could not open file '" << file_path << "'. "
                                       << strerror(err_number));
  }

  uint8_t *file_buffer = new uint8_t[m_Size];

  while (remaining_size > 0) {
    size_t to_read = 0;

#if defined(_WIN32) || defined(_WIN64)
    to_read = remaining_size;
#else
    if (remaining_size > std::numeric_limits<size_t>::max())
      to_read = std::numeric_limits<size_t>::max();
    else
      to_read = remaining_size;
#endif

    if (to_read !=
        fread(file_buffer + (m_Size - remaining_size), 1, to_read, fp)) {
      int err_number = ferror(fp);

      delete[] file_buffer;
      fclose(fp);

      FILE_ERROR("error while reading from file '" << file_path << "'. "
                                                   << strerror(err_number));
    }

    remaining_size -= to_read;
  }

  m_Chunks.emplace_back(file_buffer, m_Size);
  delete[] file_buffer;
  fclose(fp);
}

int64_t FileBuffer::get_size(void) const { return m_Size; }
const std::string &FileBuffer::get_file_path(void) const { return m_FilePath; }
void FileBuffer::change_file_path(const std::string &new_path) {
  m_FilePath = new_path;
}

void FileBuffer::set_buffer_index(int64_t index) {
  if ((0 > index && FAG_FILE_POSITION_START != index) ||
      (m_Size <= index && FAG_FILE_POSITION_END != index)) {
    std::ostringstream msg_stream;
    msg_stream << "file buffer index out of range. expected range from 0 to "
               << m_Size << ", but got " << index;

    // TODO: change to OutOfRange error
    FAG_TODO("Create an OutOfRange error class, and use that here");
    throw Error::Generic(msg_stream.str().c_str(), nullptr, 0);
  }

  if (FAG_FILE_POSITION_START == index)
    m_IndexInFile = 0;
  else if (FAG_FILE_POSITION_END == index)
    m_IndexInFile = m_Size - 1;
  else
    m_IndexInFile = index;

  return;
}
void FileBuffer::advance(int64_t byte_count) {
  int64_t new_index = m_IndexInFile + byte_count;
  if (new_index >= m_Size)
    new_index = FAG_FILE_POSITION_END;

  set_buffer_index(new_index);
}
void FileBuffer::rewind(int64_t byte_count) {
  int64_t new_index = m_IndexInFile - byte_count;
  if (new_index < 0)
    new_index = FAG_FILE_POSITION_START;

  set_buffer_index(new_index);
}

void FileBuffer::read(void *output, int64_t byte_count) const {
  uint8_t *bytes_output = static_cast<uint8_t *>(output);

  int64_t idx_in_chunk = 0;
  _FileChunks::const_iterator chunk_to_read =
      _get_current_chunk_const(&idx_in_chunk);

  int64_t remaining_byte_count = byte_count;

  while (remaining_byte_count > 0 && chunk_to_read != m_Chunks.cend()) {
    // copy loop that iterates over all the necessary chunks
    int64_t amount_to_copy =
        MIN(remaining_byte_count, (chunk_to_read->length - idx_in_chunk));
    ::memcpy(bytes_output + (byte_count - remaining_byte_count),
             chunk_to_read->data.data() + idx_in_chunk, amount_to_copy);

    idx_in_chunk = 0;
    remaining_byte_count -= amount_to_copy;

    ++chunk_to_read;
  }
}
void FileBuffer::write(void *input, int64_t byte_count) {
  uint8_t *bytes_input = static_cast<uint8_t *>(input);

  // game-plan:
  // - first we split the chunk that contains the current index into the file
  // buffer
  // - then we make *another* new chunk that will contain the newly written
  // data
  // - this chunk is then inserted in between the two original splits
  //
  //       this is where we want to write
  //               |
  //               v
  // 1: [------original-chunk-------]
  //
  // 2: [--split1--][-----split2----]
  //
  // 3: [--split1--][---we-write-here---][-----split2----]

  // step 1:
  int64_t idx_in_chunk = 0;
  _FileChunks::iterator chunk_to_split = _get_current_chunk(&idx_in_chunk);

  // step 2:

  // is this necessary?
  // splittee.data = std::vector<uint8_t>(chunk_to_split->length -
  // idx_in_chunk);

  struct _FileChunk new_chunk(bytes_input, byte_count);
  struct _FileChunk splittee(chunk_to_split->data.data() + idx_in_chunk,
                             chunk_to_split->length - idx_in_chunk);

  _FileChunks::const_iterator splittee_insert_pos = ++chunk_to_split;
  --chunk_to_split;

  _FileChunks::const_iterator splittee_iter =
      m_Chunks.insert(splittee_insert_pos, splittee);

  if (splittee_iter == chunk_to_split) {
    // failed to insert
    std::ostringstream m;
    m << "could not split file chunks for writing for some reason";
    throw Error::FileError(m.str().c_str());
  }

  // step 3:
  _FileChunks::iterator new_chunk_iter =
      m_Chunks.insert(splittee_iter, new_chunk);

  // if fail, undo everything
  if (new_chunk_iter == splittee_iter) {
    // failed to insert
    m_Chunks.erase(splittee_iter);

    std::ostringstream m;
    m << "could not insert new file chunk with new content for some reason";
    throw Error::FileError(m.str().c_str());
  }

  chunk_to_split->data.assign(chunk_to_split->data.begin(),
                              chunk_to_split->data.begin() + idx_in_chunk);
  chunk_to_split->data.shrink_to_fit();
  chunk_to_split->length = idx_in_chunk;

  m_Size += byte_count;
}
void FileBuffer::delete_bytes(int64_t byte_count) {
  if (byte_count < 0) {
    std::ostringstream msg_stream;
    msg_stream << "byte count for deletion out of range. expected range from 0 "
                  "to inf, but got "
               << byte_count;

    // TODO: change to OutOfRange error
    FAG_TODO("Create an OutOfRange error class, and use that here");
    throw Error::Generic(msg_stream.str().c_str(), nullptr, 0);
  }

  int64_t idx_in_chunk = 0;
  _FileChunks::iterator current_chunk = _get_current_chunk(&idx_in_chunk);

  if (idx_in_chunk > 0) {
    if (byte_count + idx_in_chunk >= current_chunk->length) {
      // easy, we just resize the vector
      current_chunk->data.assign(current_chunk->data.data(),
                                 current_chunk->data.data() + idx_in_chunk);
      byte_count -= (current_chunk->length - idx_in_chunk);
      m_Size -= (current_chunk->length - idx_in_chunk);

      current_chunk->data.shrink_to_fit();
      current_chunk->length = idx_in_chunk;
      ++current_chunk;
    } else {
      // we reassign the vector to only contain the data that was already around
      // the deleted block
      int64_t end_bytes_remaining = current_chunk->length - byte_count;
      int64_t continue_offset = idx_in_chunk + byte_count;

      if (continue_offset < idx_in_chunk) {
        // integer overflow
        // TODO: change to IntegerOverflow error
        FAG_TODO("Create an IntegerOverflow error class, and use that here");
        throw Error::Generic(
            "integer overflow when attempting to delete bytes from file buffer",
            nullptr, 0);
      }

      uint8_t *temp_buffer = new uint8_t[end_bytes_remaining]{};
      ::memcpy(temp_buffer, current_chunk->data.data(), idx_in_chunk);
      ::memcpy(temp_buffer + idx_in_chunk,
               current_chunk->data.data() + (continue_offset),
               end_bytes_remaining - idx_in_chunk);

      current_chunk->data.assign(temp_buffer,
                                 temp_buffer + end_bytes_remaining);
      current_chunk->data.shrink_to_fit();
      current_chunk->length = end_bytes_remaining;
      m_Size -= byte_count;
      delete[] temp_buffer;
      return;
    }
  }

  _FileChunks::iterator starting_chunk = current_chunk;

  while (byte_count > current_chunk->length &&
         current_chunk != m_Chunks.end()) {
    // we want to delete more than this entire chunk.
    // bye bye chunk!!!!!

    byte_count -= current_chunk->length;
    m_Size -= current_chunk->length;
    ++current_chunk;
  }

  _FileChunks::iterator last_chunk =
      m_Chunks.erase(starting_chunk, current_chunk);

  if (last_chunk == m_Chunks.end())
    return;

  int64_t newlen = last_chunk->length - byte_count;
  uint8_t *newdata = last_chunk->data.data() + byte_count;
  last_chunk->data.assign(newdata, newdata + newlen);
  last_chunk->data.shrink_to_fit();
  last_chunk->length = newlen;
  m_Size -= byte_count;
}

void FileBuffer::save_to_disk(void) {
  if (!(static_cast<uint8_t>(accessMode) |
        static_cast<uint8_t>(FileAccessMode::Write))) {
    FILE_ERROR("filebuffer for '"
               << m_FilePath
               << "' was not opened with the Write flag set. aborting write");
  }
  _save_to_disk(m_FilePath);
}
void FileBuffer::save_to_disk(const std::string &path) {
  try {
    _save_to_disk(path);
    m_FilePath = path;
  } catch (const Error::FileError &err) {
    throw err;
  }
}

void FileBuffer::_save_to_disk(const std::string &path) {
  uint8_t *byte_buffer = new uint8_t[m_Size];
  int64_t remaining_to_write = 0;

  for (const struct _FileChunk &chunk : m_Chunks) {
    ::memcpy(byte_buffer + remaining_to_write, chunk.data.data(), chunk.length);
    remaining_to_write += chunk.length;
  }

  assert(remaining_to_write == m_Size);

  FILE *fp = fopen(path.c_str(), "wb");
  if (nullptr == fp) {
    int err_num = errno;
    FILE_ERROR("failed to open file '" << path << "' for writing. "
                                       << strerror(err_num));
  }

  while (remaining_to_write > 0) {
    size_t to_write = 0;

#if defined(_WIN32) || defined(_WIN64)
    to_write = remaining_to_write;
#else
    if (remaining_to_write > std::numeric_limits<size_t>::max())
      to_write = std::numeric_limits<size_t>::max();
    else
      to_write = remaining_to_write;
#endif

    if (to_write !=
        fwrite(byte_buffer + (m_Size - remaining_to_write), 1, to_write, fp)) {
      int err_number = ferror(fp);

      fclose(fp);

      delete[] byte_buffer;

      FILE_ERROR("failed to write to open file '" << path << "'. "
                                                  << strerror(err_number));
    }

    remaining_to_write -= to_write;
  }

  delete[] byte_buffer;
}

#define TRAVERSE_CHUNKS(_chunk_var, _idx_output)                               \
  {                                                                            \
    int64_t traversed = 0;                                                     \
    for (; return_chunk != m_Chunks.end(); ++return_chunk) {                   \
      if (traversed + return_chunk->length > m_IndexInFile)                    \
        break;                                                                 \
                                                                               \
      traversed += return_chunk->length;                                       \
      continue;                                                                \
    }                                                                          \
                                                                               \
    if (m_Chunks.end() == return_chunk) {                                      \
      /* Index is out of range */                                              \
                                                                               \
      std::ostringstream m;                                                    \
      m << "internal FileBuffer index is out of range for some reason";        \
      throw Error::FileError(m.str().c_str());                                 \
    }                                                                          \
    _idx_output = (m_IndexInFile - traversed);                                 \
  }

FileBuffer::_FileChunks::iterator
FileBuffer::_get_current_chunk(int64_t *idx_within_chunk_output) {
  _FileChunks::iterator return_chunk = m_Chunks.begin();

  TRAVERSE_CHUNKS(return_chunk, *idx_within_chunk_output);

  return return_chunk;
}

FileBuffer::_FileChunks::const_iterator
FileBuffer::_get_current_chunk_const(int64_t *idx_within_chunk_output) const {
  _FileChunks::const_iterator return_chunk = m_Chunks.cbegin();

  TRAVERSE_CHUNKS(return_chunk, *idx_within_chunk_output);

  return return_chunk;
}

FileBuffer::_FileChunk::_FileChunk(const uint8_t *dataptr, int64_t len) {
  data.assign(dataptr, dataptr + len);
  length = len;
}

}; // namespace OS
}; // namespace fag
