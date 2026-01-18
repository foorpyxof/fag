/*
 * Copyright (c) Erynn Scholtes
 * SPDX-License-Identifier: MIT
 */

#ifndef FAG_DEBUG_H
#define FAG_DEBUG_H

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#define LONG_FORMAT "ll"
#else
#define LONG_FORMAT "l"
#endif

#define FAG_DEBUG(component, fmt, ...)
#define FAG_WARN(component, fmt, ...)
#define FAG_ERROR(component, fmt, ...)
#define FAG_TODO(fmt, ...)

#define FAG_LINE_INFO(output)

#if (defined __FILE__ && defined __LINE__)

#define GREEN_TEXT "\033[0;92m"
#define YELLOW_TEXT "\033[0;93m"
#define RED_TEXT "\033[0;91m"
#define BLUE_TEXT "\033[0;96m"
#define COLOR_RESET "\033[0m"

#undef FAG_LINE_INFO
#define FAG_LINE_INFO(output) sprintf(output, "%s:%d", __FILE__, __LINE__)

#endif // __FILE__ && __LINE__

#ifdef FAG_DEBUG_ENABLE

#undef FAG_DEBUG
#define FAG_DEBUG(component, fmt, ...)                                         \
  fprintf(stderr,                                                              \
          GREEN_TEXT "F.A.G. DEBUG: " BLUE_TEXT #component COLOR_RESET         \
                     " -> " fmt COLOR_RESET "\n" __VA_OPT__(, ) __VA_ARGS__)

#undef FAG_WARN
#define FAG_WARN(component, fmt, ...)                                          \
  fprintf(stderr,                                                              \
          YELLOW_TEXT "F.A.G. WARN: " BLUE_TEXT #component COLOR_RESET         \
                      " -> " fmt COLOR_RESET "\n" __VA_OPT__(, ) __VA_ARGS__)

#undef FAG_TODO
#define FAG_TODO(fmt, ...)                                                     \
  {                                                                            \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    fprintf(stderr,                                                            \
            BLUE_TEXT "F.A.G. TODO: " COLOR_RESET fmt COLOR_RESET              \
                      " (at %s)\n" __VA_OPT__(, ) __VA_ARGS__,                 \
            _fag_lineinfo_output_buffer);                                      \
  }

#endif // FAG_DEBUG_ENABLE

#if defined(FAG_DEBUG_ENABLE) || !defined(FAG_SILENT_ERROR)
#undef FAG_ERROR
#define FAG_ERROR(component, fmt, ...)                                         \
  {                                                                            \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    fprintf(stderr,                                                            \
            RED_TEXT "F.A.G. ERROR: " BLUE_TEXT #component COLOR_RESET         \
                     " -> " fmt COLOR_RESET " (at %s)\n" __VA_OPT__(, )        \
                         __VA_ARGS__,                                          \
            _fag_lineinfo_output_buffer);                                      \
  }
#endif // FAG_DEBUG_ENABLE || !FAG_SILENT_ERROR

#undef LIBRARY_NAME

#endif // FAG_DEBUG_H
