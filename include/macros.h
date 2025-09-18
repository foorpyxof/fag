/*
 * Copyright (c) Erynn Scholtes
 * SPDX-License-Identifier: MIT
 */

#ifndef FAG_MACROS_H
#define FAG_MACROS_H

#include "debug.h"

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

#define NULL_CHECK(_var, _if_null)                                             \
  if (NULL == _var) {                                                          \
    _if_null;                                                                  \
  }

#endif // FAG_MACROS_H
