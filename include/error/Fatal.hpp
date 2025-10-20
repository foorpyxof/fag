#ifndef FAG_ERROR_FATAL_HPP
#define FAG_ERROR_FATAL_HPP

#include <string>

namespace fag {
namespace Error {

class Fatal {
public:
  Fatal(const char *message, const char *file, int line);

public:
  const char *what() const noexcept;

private:
  std::string m_Message;
};

} // namespace Error
} // namespace fag

#endif // FAG_ERROR_FATAL_HPP
