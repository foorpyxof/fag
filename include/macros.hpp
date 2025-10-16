/*
 * Copyright (c) Erynn Scholtes
 * SPDX-License-Identifier: MIT
 */

#ifndef FAG_MACROS_H
#define FAG_MACROS_H

#include "./general.hpp"
#include "debug.h"

#include <stdexcept>
#include <string>

extern "C" {

#ifdef IMPLEMENT_THIS
#undef IMPLEMENT_THIS
#endif
#define IMPLEMENT_THIS(_func, _body)                                           \
  _func { _body FAG_TODO("Implement \"%s\"", #_func); }

#ifdef UNUSED
#undef UNUSED
#endif
#define UNUSED(_var)                                                           \
  {                                                                            \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    FAG_DEBUG("Variable %s is unused (at: %s)", #_var,                         \
              _fag_lineinfo_output_buffer);                                    \
    if (&_var) {                                                               \
    }                                                                          \
  }

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef NULL_CHECK
#undef NULL_CHECK
#endif
#define NULL_CHECK(_var, _if_null)                                             \
  if (NULL == _var) {                                                          \
    _if_null;                                                                  \
  }

#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

#ifdef MAX
#undef MAX
#endif
#define MAX(_a, _b) ((_a > _b) ? (_a) : (_b))

#ifdef MIN
#undef MIN
#endif
#define MIN(_a, _b) ((_a < _b) ? (_a) : (_b))
}

#ifdef THROW_ON_FAIL
#undef THROW_ON_FAIL
#endif
#define THROW_ON_FAIL(_result)                                                 \
  if (_result < fag::Result::Success) {                                        \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    const char *_temp_msg = "A function failed at runtime. (at: ";             \
    std::string message = _temp_msg;                                           \
    message.append(_fag_lineinfo_output_buffer);                               \
    message.append(")");                                                       \
    throw std::runtime_error(message);                                         \
  }

#endif // FAG_MACROS_H
