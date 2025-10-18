// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_DEV_ALLOCATORS_HPP
#define FAG_DEV_ALLOCATORS_HPP

#include "../core/Engine.hpp"
#include "../core/typedefs.h"

#define FAG_CUSTOM_ALLOCATOR fag::Engine::get_custom_allocator()

#define FAG_HEAP_CONSTRUCT(_type, _ptr_var_name, _ctor)                        \
  _ptr_var_name = (FAG_CUSTOM_ALLOCATOR.is_valid()                             \
                       ? (static_cast<_type *>(                                \
                             FAG_CUSTOM_ALLOCATOR.alloc_func(sizeof(_type))))  \
                       : new _type _ctor);                                     \
  if (FAG_CUSTOM_ALLOCATOR.is_valid())                                         \
  *_ptr_var_name = _type _ctor

#define FAG_HEAP_DESTRUCT(_type, _ptr_var_name)                                \
  if (FAG_CUSTOM_ALLOCATOR.is_valid()) {                                       \
    _ptr_var_name->~_type();                                                   \
    FAG_CUSTOM_ALLOCATOR.free_func(_ptr_var_name);                             \
  } else                                                                       \
    delete _ptr_var_name

#endif // FAG_DEV_ALLOCATORS_HPP
