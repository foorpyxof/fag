#ifndef FAG_ERROR_FILEERROR_HPP
#define FAG_ERROR_FILEERROR_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class FileError : public IError {
public:
  const char *what() const noexcept override;

public:
  FileError(const char *message);

private:
  std::string m_Message;
};

} // namespace Error
} // namespace fag

#endif // FAG_ERROR_FILEERROR_HPP
