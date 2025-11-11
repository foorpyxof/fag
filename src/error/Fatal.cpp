// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include <sstream>

#include "error/Fatal.hpp"

namespace fag {
namespace Error {

#define ERROR_MESSAGE "A fatal error has occured."

const char *Fatal::what(void) const noexcept { return m_Message.c_str(); }

Fatal::Fatal(const char *msg, const char *file, int line) {
  std::ostringstream full_msg;

  full_msg << ERROR_MESSAGE;

  if (!msg || !file || line < 1) {
    std::ostringstream err_msg;

    err_msg << "The throwing of an ill-formed fatal error was attempted";
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
