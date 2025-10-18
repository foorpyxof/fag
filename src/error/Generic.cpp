// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "error/Generic.hpp"

#include <sstream>

namespace fag {

namespace Error {

#define ERROR_MESSAGE "An error has occured at runtime."

Generic::Generic() : Generic(nullptr) {}
Generic::Generic(const char *msg) : Generic(msg, nullptr, 0) {}
Generic::Generic(const char *msg, const char *file, int line) {
  std::ostringstream full_msg;

  if (nullptr != msg)
    full_msg << msg;
  else
    full_msg << ERROR_MESSAGE;

  if (nullptr != file) {
    full_msg << "\n(at: " << file;

    if (line > 0)
      full_msg << ":" << std::to_string(line);

    full_msg << ")";
  }

  m_Message = full_msg.str();
}

const char *Generic::what() const noexcept { return m_Message.c_str(); }

}; // namespace Error

}; // namespace fag
