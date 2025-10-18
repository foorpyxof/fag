// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_ALLOCATOR_HPP
#define FAG_CORE_ALLOCATOR_HPP

#include <cstddef>

namespace fag {

typedef void *(*AllocationFunction)(size_t);
typedef void (*FreeFunction)(void *);

typedef struct fag_allocator_struct {
  AllocationFunction alloc_func;
  FreeFunction free_func;

  bool is_valid() const;
} Allocator;

} // namespace fag

#endif // FAG_CORE_ALLOCATOR_HPP
