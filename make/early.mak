COMPILE_FLAGS := -Wall -Wextra -Wpedantic -Werror -Wno-gnu-zero-variadic-macro-arguments -Wno-unknown-warning-option -Wno-variadic-macro-arguments-omitted

CFLAGS := -std=gnu11 $(COMPILE_FLAGS)
CPPFLAGS := -std=c++20 $(COMPILE_FLAGS)
LDFLAGS := -lm

ROOT_DIR != pwd
ROOT_DIR_NAME != basename $(ROOT_DIR)
