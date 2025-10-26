#include "error/Fatal.hpp"

#include <sstream>

#include "error/Internal.hpp"

namespace fag {
namespace Error {

#define ERROR_MESSAGE "An internal error has occured in the F.A.G. engine."

const char *Internal::what(void) const noexcept { return m_Message.c_str(); }

Internal::Internal(const char *msg, const char *file, int line) {
  std::ostringstream full_msg;
  full_msg << ERROR_MESSAGE;

  if (!msg || !file || line < 1) {
    std::ostringstream err_msg;

    err_msg << "The throwing of an ill-formed internal error was attempted";
    if (file && line > 0)
      err_msg << " at " << __FILE__ << ":" << __LINE__;
    else
      err_msg << " somewhere, but unfortunately we don't know from where";

    throw Fatal(err_msg.str().c_str(), __FILE__, __LINE__);
  }

  full_msg << "\n" << msg << "\n(at: " << file << ":" << line << ")";

  m_Message = full_msg.str();
}

} // namespace Error
} // namespace fag
