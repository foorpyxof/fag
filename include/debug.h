#ifndef FAG_DEBUG_H
#define FAG_DEBUG_H

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#define LONG_FORMAT "ll"
#else
#define LONG_FORMAT "l"
#endif

#define FAG_DEBUG(fmt, ...)
#define FAG_WARN(fmt, ...)
#define FAG_ERROR(fmt, ...)
#define FAG_TODO(fmt, ...)

#define FAG_LINE_INFO(output)

#if (defined __FILE__ && defined __LINE__)

#undef FAG_LINE_INFO
#define FAG_LINE_INFO(output) sprintf(output, "%s:%d", __FILE__, __LINE__)

#endif // __FILE__ && __LINE__

#ifdef FAG_DEBUG_ENABLE

#undef FAG_DEBUG
#define FAG_DEBUG(fmt, ...)                                                    \
  fprintf(stderr, "\033[0;92mF.A.G. DEBUG:\033[0m " fmt                        \
                  "\033[0m\n" __VA_OPT__(, ) __VA_ARGS__)

#undef FAG_WARN
#define FAG_WARN(fmt, ...)                                                     \
  fprintf(stderr, "\033[0;93mF.A.G. WARN: \033[0m " fmt                        \
                  "\033[0m\n" __VA_OPT__(, ) __VA_ARGS__)

#undef FAG_TODO
#define FAG_TODO(fmt, ...)                                                     \
  {                                                                            \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    fprintf(stderr,                                                            \
            "\033[0;96mF.A.G. TODO: \033[0m " fmt                              \
            "\033[0m (at %s)\n" __VA_OPT__(, ) __VA_ARGS__,                    \
            _fag_lineinfo_output_buffer);                                      \
  }

#endif // FAG_DEBUG_ENABLE

#if defined(FAG_DEBUG_ENABLE) || !defined(FAG_SILENT_ERROR)
#undef FAG_ERROR
#define FAG_ERROR(fmt, ...)                                                    \
  {                                                                            \
    char _fag_lineinfo_output_buffer[sizeof(__FILE__) + 16];                   \
    FAG_LINE_INFO(_fag_lineinfo_output_buffer);                                \
    fprintf(stderr,                                                            \
            "\033[0;91mF.A.G. ERROR:\033[0m " fmt                              \
            "\033[0m (at %s)\n" __VA_OPT__(, ) __VA_ARGS__,                    \
            _fag_lineinfo_output_buffer);                                      \
  }
#endif // FAG_DEBUG_ENABLE || !FAG_SILENT_ERROR

#undef LIBRARY_NAME

#endif // FAG_DEBUG_H
