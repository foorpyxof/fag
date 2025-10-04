/*
 * Copyright (c) Erynn Scholtes
 * SPDX-License-Identifier: MIT
 */

#ifndef FAG_MACROS_H
#define FAG_MACROS_H

#include "debug.h"

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

#endif // FAG_MACROS_H
