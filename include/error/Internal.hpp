#ifndef FAG_ERROR_INTERNAL_HPP
#define FAG_ERROR_INTERNAL_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class Internal : public IError {
public:
  Internal(const char *message, const char *filename, int line_number);

public:
  const char *what() const noexcept;

private:
  std::string m_Message;
};

} // namespace Error
} // namespace fag

#endif // FAG_ERROR_INTERNAL_HPP
