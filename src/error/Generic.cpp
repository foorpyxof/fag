// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include <sstream>

#include "error/Generic.hpp"

namespace fag {
namespace Error {

#define ERROR_MESSAGE "An error has occured at runtime."

const char *Generic::what() const noexcept { return m_Message.c_str(); }

Generic::Generic() : Generic(nullptr) {}
Generic::Generic(const char *msg) : Generic(msg, nullptr, 0) {}
Generic::Generic(const char *msg, const char *file, int line) {
  std::ostringstream full_msg;
  full_msg << ERROR_MESSAGE;

  if (nullptr != msg)
    full_msg << "\n" << msg;

  if (nullptr != file && line > 0)
    full_msg << "\n(at: " << file << ":" << std::to_string(line) << ")";

  m_Message = full_msg.str();
}

}; // namespace Error
}; // namespace fag
