#ifndef FAG_EXCEPTION_GENERIC_HPP
#define FAG_EXCEPTION_GENERIC_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class Generic : public IError {
public:
  Generic();
  Generic(const char *message);
  Generic(const char *message, const char *filename, int line_number);

public:
  const char *what() const noexcept;

private:
  std::string m_Message;
};

}; // namespace Error
}; // namespace fag

#endif // FAG_EXCEPTION_GENERIC_HPP
