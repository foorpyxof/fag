// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ERROR_INTERNAL_HPP
#define FAG_ERROR_INTERNAL_HPP

#include "./IError.hpp"

#include <string>

namespace fag {
namespace Error {

class Internal : public IError {
public:
  const char *what() const noexcept override;

public:
  Internal(const char *message, const char *filename, int line_number);

private:
  std::string m_Message;
};

} // namespace Error
} // namespace fag

#endif // FAG_ERROR_INTERNAL_HPP
