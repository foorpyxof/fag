// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_EXCEPTION_MEMORY_HPP
#define FAG_EXCEPTION_MEMORY_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class Memory : public IError {
public:
  const char *what() const noexcept override;

public:
  Memory();
  Memory(const char *message);
  Memory(const char *message, const char *filename, int line_number);

private:
  std::string m_Message;
};

}; // namespace Error
}; // namespace fag

#endif // FAG_EXCEPTION_MEMORY_HPP
