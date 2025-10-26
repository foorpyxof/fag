// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_EXCEPTION_IEXCEPTION_HPP
#define FAG_EXCEPTION_IEXCEPTION_HPP

#include <exception>

namespace fag {
namespace Error {

class IError : std::exception {
public:
  virtual ~IError() {}
};

}; // namespace Error
}; // namespace fag

#endif // FAG_EXCEPTION_IEXCEPTION_HPP
