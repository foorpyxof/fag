// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_DEV_ALLOCATORS_HPP
#define FAG_DEV_ALLOCATORS_HPP

#include "../core/Globals.hpp"
#include "../core/typedefs.h"
#include "../debug.h"

#include <cstring>
#include <new>

/*
 *
 * WE DON'T ACTUALLY USE THE CUSTOM ALLOCATOR+DELETER YET
 * UNTIL WE FIGURE OUT HOW TO GIVE THE DELETER TO SMART POINTERS
 *
 */

#define FAG_CUSTOM_ALLOCATOR g_Engine->get_custom_allocator()

// #define FAG_HEAP_CONSTRUCT(_type, _ptr_var_name, _ctor) \
//   _ptr_var_name = (FAG_CUSTOM_ALLOCATOR.is_valid() \
//                        ? (static_cast<_type *>( \
//                              FAG_CUSTOM_ALLOCATOR.alloc_func(sizeof(_type))))
//                              \
//                        : new (std::nothrow) _type _ctor); \
//   if (!_ptr_var_name) { \
//     FAG_TODO("handle errors in macro FAG_HEAP_CONSTRUCT"); \
//   } else if (FAG_CUSTOM_ALLOCATOR.is_valid()) { \
//     _type _fag_temp_var _ctor; \
//     ::memcpy(static_cast<void *>(_ptr_var_name), \
//              static_cast<void *>(&_fag_temp_var), sizeof(_type)); \
//   }

#define FAG_HEAP_CONSTRUCT(_type, _ptr_var_name, _ctor)                        \
  try {                                                                        \
    _ptr_var_name = new _type _ctor;                                           \
  } catch (const std::bad_alloc &_ba_exc) {                                    \
    FAG_TODO("handle errors in macro FAG_HEAP_CONSTRUCT");                     \
  }

// #define FAG_HEAP_DESTRUCT(_type, _ptr_var_name) \
//   if (FAG_CUSTOM_ALLOCATOR.is_valid()) { \
//     _ptr_var_name->~_type(); \
//     FAG_CUSTOM_ALLOCATOR.free_func(_ptr_var_name); \
//   } else \
//     delete _ptr_var_name;

#define FAG_HEAP_DESTRUCT(_type, _ptr_var_name) delete _ptr_var_name

#endif // FAG_DEV_ALLOCATORS_HPP
