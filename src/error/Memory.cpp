// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include <sstream>

#include "error/Memory.hpp"

namespace fag {
namespace Error {

#define ERROR_MESSAGE "A memory error has occured at runtime."

const char *Memory::what() const noexcept { return m_Message.c_str(); }

Memory::Memory() : Memory(nullptr) {}
Memory::Memory(const char *msg) : Memory(msg, nullptr, 0) {}
Memory::Memory(const char *msg, const char *file, int line) {
  std::ostringstream full_msg;
  full_msg << ERROR_MESSAGE;

  if (nullptr != msg)
    full_msg << "\n" << msg;

  if (nullptr != file && line > 0)
    full_msg << "\n(at: " << file << ":" << line << ")";

  m_Message = full_msg.str();
}

}; // namespace Error
}; // namespace fag
