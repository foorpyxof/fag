// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_OS_FILE_HPP
#define FAG_OS_FILE_HPP

#include <cstdint>
#include <limits>
#include <list>
#include <string>
#include <vector>

#define FAG_FILE_POSITION_START (std::numeric_limits<int64_t>::min())
#define FAG_FILE_POSITION_END (std::numeric_limits<int64_t>::max())

namespace fag {
namespace OS {

enum class FileAccessMode : uint8_t;

class FileBuffer {
public:
  int64_t get_size(void) const;
  const std::string &get_file_path(void) const;
  void change_file_path(const std::string &new_path);

  // navigate the buffer's internal byte index
  // you can use `FAG_FILE_POSITION_START`
  // and `FAG_FILE_POSITION_END` macros here
  void set_buffer_index(int64_t index);
  void advance(int64_t byte_count);
  void rewind(int64_t byte_count);

  // read, write and delete from buffer memory
  // (this does not commit changes to the filesystem)
  void read(void *output, int64_t byte_count) const;
  void write(void *input, int64_t byte_count);
  void delete_bytes(int64_t byte_count);

  // save current buffer changes to disk
  // (commits writes and deletions to filesystem)
  void save_to_disk(void);
  // possibly under a new file path, which will automatically
  // associate this buffer with that new path
  void save_to_disk(const std::string &path);

public:
  FileBuffer(const std::string &path, FileAccessMode);

public:
  const FileAccessMode accessMode;

private:
  struct _FileChunk;
  typedef std::list<_FileChunk> _FileChunks;

private:
  /**
   * returns 0 on success;
   * returns errno on failure;
   */
  void _save_to_disk(const std::string &path);

  _FileChunks::iterator _get_current_chunk(int64_t *idx_in_chunk_output);
  _FileChunks::const_iterator
  _get_current_chunk_const(int64_t *idx_in_chunk_output) const;

private:
  std::string m_FilePath;
  int64_t m_IndexInFile;
  int64_t m_Size;

  _FileChunks m_Chunks;

private:
  struct _FileChunk {
    std::vector<uint8_t> data;
    int64_t length;

    _FileChunk(const uint8_t *dataptr, int64_t len);
  };
};

enum class FileAccessMode : uint8_t {
  Read = 0x04,  // 0b100
  Write = 0x02, // 0b010
  ReadWrite = Read | Write
};

}; // namespace OS
}; // namespace fag

#endif // FAG_OS_FILE_HPP
