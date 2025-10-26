// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_EXCEPTION_GENERIC_HPP
#define FAG_EXCEPTION_GENERIC_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class Generic : public IError {
public:
  const char *what() const noexcept;

public:
  Generic();
  Generic(const char *message);
  Generic(const char *message, const char *filename, int line_number);

private:
  std::string m_Message;
};

}; // namespace Error
}; // namespace fag

#endif // FAG_EXCEPTION_GENERIC_HPP
