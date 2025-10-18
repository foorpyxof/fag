// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Allocator.hpp"

namespace fag {

bool Allocator::is_valid() const { return (alloc_func && free_func); }

} // namespace fag
