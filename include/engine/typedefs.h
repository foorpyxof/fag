/*
 * Copyright (c) Erynn Scholtes
 * SPDX-License-Identifier: MIT
 */

#ifndef FAG_ENGINE_TYPEDEFS_HPP
#define FAG_ENGINE_TYPEDEFS_HPP

#include <cstddef>

namespace fag {

typedef void *(*AllocationFunction)(size_t);
typedef void (*FreeFunction)(void *);

typedef struct {
  AllocationFunction allocFunc;
  FreeFunction freeFunc;
} Allocator;

} // namespace fag

#endif // FAG_ENGINE_TYPEDEFS_HPP
