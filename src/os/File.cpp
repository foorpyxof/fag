// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "os/File.hpp"
#include "error/Generic.hpp"

#include "debug.h"
#include "error/IError.hpp"
#include "macros.hpp"

#include <cstring>
#include <sstream>
#include <string>

namespace fag {
namespace OS {

FileBuffer::FileBuffer(std::string &filename, FileAccessMode mode)
    : name(filename), accessMode(mode) {}

int64_t FileBuffer::get_size() const { return m_Size; }

void FileBuffer::set_buffer_index(int64_t index) {
  if ((0 > index && FAG_FILE_POSITION_START != index) ||
      (m_Size <= index && FAG_FILE_POSITION_END != index)) {
    std::ostringstream msg_stream;
    msg_stream << "File buffer index out of range.\nGot: '" << index
               << "'\nExpected range from 0 to " << m_Size << ", but got "
               << index << ".";

    // TODO: change to OutOfRange error
    FAG_TODO("Create an OutOfRange error class, and use that here");
    throw Error::Generic(msg_stream.str().c_str(), nullptr, 0);
  }

  if (FAG_FILE_POSITION_START == index)
    m_Index = 0;
  else if (FAG_FILE_POSITION_END == index)
    m_Index = m_Size - 1;

  return;
}

void FileBuffer::advance(int64_t byte_count) {
  int64_t new_index = m_Index + byte_count;
  if (new_index >= m_Size)
    new_index = FAG_FILE_POSITION_END;

  set_buffer_index(new_index);
}

void FileBuffer::rewind(int64_t byte_count) {
  int64_t new_index = m_Index - byte_count;
  if (new_index < 0)
    new_index = FAG_FILE_POSITION_START;

  set_buffer_index(new_index);
}

void FileBuffer::read(void *output, int64_t byte_count) {
  int64_t traversed;
  uint8_t *bytes_output = static_cast<uint8_t *>(output);

  std::list<_FileChunk>::const_iterator chunk_to_read =
      _get_current_chunk(&traversed);

  int64_t idx_in_chunk = m_Index - traversed;
  int64_t remaining_byte_count = byte_count;

  while (remaining_byte_count > 0) {
    // copy loop that iterates over all the necessary chunks
    int64_t amount_to_copy =
        MIN(remaining_byte_count, (chunk_to_read->length - idx_in_chunk));
    ::memcpy(bytes_output + (byte_count - remaining_byte_count),
             chunk_to_read->data.data() + idx_in_chunk, amount_to_copy);

    idx_in_chunk = 0;
    remaining_byte_count -= amount_to_copy;
  }
}

void FileBuffer::write(void *input, int64_t byte_count) {
  int64_t traversed;
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
  std::list<_FileChunk>::iterator chunk_to_split =
      _get_current_chunk(&traversed);

  int64_t idx_in_chunk = m_Index - traversed;

  // step 2:

  // is this necessary?
  // splittee.data = std::vector<uint8_t>(chunk_to_split->length -
  // idx_in_chunk);

  struct _FileChunk new_chunk(bytes_input, byte_count);
  struct _FileChunk splittee(chunk_to_split->data.data() + idx_in_chunk,
                             chunk_to_split->length - idx_in_chunk);

  std::list<_FileChunk>::iterator splittee_insert_pos = ++chunk_to_split;
  --chunk_to_split;

  std::list<_FileChunk>::iterator splittee_iter =
      m_Chunks.insert(splittee_insert_pos, splittee);

  // if fail, undo everything
  if (splittee_iter == chunk_to_split) {
    // failed to insert
    std::ostringstream m;
    m << "Could not split file chunks for writing for some reason";
    FAG_TODO("Create an Internal error class, and use that here");
    throw Error::Generic(m.str().c_str(), nullptr, 0);
  }

  // step 3:
  std::list<_FileChunk>::iterator new_chunk_iter =
      m_Chunks.insert(splittee_iter, new_chunk);

  // if fail, undo everything
  if (new_chunk_iter == splittee_iter) {
    // failed to insert
    m_Chunks.erase(splittee_iter);

    std::ostringstream m;
    m << "Could not insert new file chunk with new content for some reason";
    FAG_TODO("Create an Internal error class, and use that here");
    throw Error::Generic(m.str().c_str(), nullptr, 0);
  }

  chunk_to_split->data.assign(chunk_to_split->data.begin(),
                              chunk_to_split->data.begin() + idx_in_chunk);
  chunk_to_split->length = idx_in_chunk;

  m_Size += byte_count;
}

std::list<FileBuffer::_FileChunk>::iterator
FileBuffer::_get_current_chunk(int64_t *traversed_count_output) {
  int64_t traversed = 0;

  std::list<_FileChunk>::iterator return_chunk = m_Chunks.end();

  for (; return_chunk != m_Chunks.end(); ++return_chunk) {
    if (traversed + return_chunk->length > m_Index)
      break;

    traversed += return_chunk->length;
    continue;
  }

  if (m_Chunks.cbegin() == return_chunk) {
    // Index is out of range

    std::ostringstream m;
    m << "Internal FileBuffer index is out of range for some reason";

    // TODO: change to Internal error, because erroring here means that earlier
    // internal logic executed incorrectly
    FAG_TODO("Create an Internal error class, and use that here");
    throw Error::Generic(m.str().c_str(), nullptr, 0);
  }

  *traversed_count_output = traversed;
  return return_chunk;
}

FileBuffer::_FileChunk::_FileChunk(uint8_t *dataptr, int64_t len) {
  data.assign(dataptr, dataptr + len);
  length = len;
}

}; // namespace OS
}; // namespace fag
