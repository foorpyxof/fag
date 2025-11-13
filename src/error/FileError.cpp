// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include <sstream>

#include "error/FileError.hpp"

namespace fag {
namespace Error {

#define ERROR_MESSAGE "An error related to file I/O has occured"

const char *FileError::what(void) const noexcept { return m_Message.c_str(); }

FileError::FileError(const char *msg) {
  std::ostringstream full_msg;
  full_msg << ERROR_MESSAGE << "\n" << msg;
  m_Message = full_msg.str();
}

} // namespace Error
} // namespace fag
