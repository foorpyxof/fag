COMPILE_FLAGS := -Wall -Wextra -Wpedantic -Werror -Wno-gnu-zero-variadic-macro-arguments -Wno-unknown-warning-option -Wno-variadic-macro-arguments-omitted -Wno-comment

CFLAGS := -std=c11 $(COMPILE_FLAGS)
CPPFLAGS := -std=c++20 $(COMPILE_FLAGS)
# i hate that we have to use c++20 instead of c++11, but the __VA_ARGS__ macro doesn't exist in mingw-c++11 apparently.......

LDFLAGS := -lm

ROOT_DIR != pwd
ROOT_DIR_NAME != basename $(ROOT_DIR)
