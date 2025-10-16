#ifndef FAG_OS_FILE_HPP
#define FAG_OS_FILE_HPP

#include <cstdint>
#include <limits>
#include <list>
#include <string>
#include <vector>

namespace fag {
namespace OS {

enum class FileAccessMode {
  Read = 0x01,
  Write = 0x02,
  ReadWrite = Read | Write
};

class FileBuffer {
public:
  FileBuffer(std::string &, FileAccessMode);

public:
  int64_t get_size() const;

#define FAG_FILE_POSITION_START (std::numeric_limits<int64_t>::min())
#define FAG_FILE_POSITION_END (std::numeric_limits<int64_t>::max())
  void set_buffer_index(int64_t index);

  void advance(int64_t byte_count);
  void rewind(int64_t byte_count);

  void read(void *output, int64_t byte_count);
  void write(void *input, int64_t byte_count);

public:
  const std::string name;
  const FileAccessMode accessMode;

private:
  struct _FileChunk {
    _FileChunk(uint8_t *dataptr, int64_t len);

    std::vector<uint8_t> data;
    int64_t length;
  };

private:
  std::list<_FileChunk>::iterator
  _get_current_chunk(int64_t *traversed_count_output);

private:
  int64_t m_Size;
  int64_t m_Index;

  std::list<_FileChunk> m_Chunks;
};

}; // namespace OS
}; // namespace fag

#endif // FAG_OS_FILE_HPP
