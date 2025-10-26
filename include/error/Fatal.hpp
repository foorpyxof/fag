#ifndef FAG_ERROR_FATAL_HPP
#define FAG_ERROR_FATAL_HPP

#include <string>

namespace fag {
namespace Error {

class Fatal {
public:
  const char *what() const noexcept;

public:
  Fatal(const char *message, const char *file, int line);

private:
  std::string m_Message;
};

} // namespace Error
} // namespace fag

#endif // FAG_ERROR_FATAL_HPP
