// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_GENERAL_HPP
#define FAG_GENERAL_HPP

#include <cstdint>

namespace fag {

enum class Result : int32_t {
  Success = 0,
  GeneralFailure = -1,
};

} // namespace fag

#endif // FAG_GENERAL_HPP
