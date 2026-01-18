// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_DEV_SMARTPTRS_HPP
#define FAG_DEV_SMARTPTRS_HPP

#include "./allocators.hpp"

#include <memory>

namespace fag {
namespace _dev {

template <typename T>
constexpr ::std::shared_ptr<T> create_shared_ptr(T *_src_ptr) {
  return ::std::shared_ptr<T>{_src_ptr};
}
template <typename T>
constexpr ::std::weak_ptr<T> create_weak_ptr(::std::shared_ptr<T> &_src_ptr) {
  return ::std::weak_ptr<T>{_src_ptr};
}
template <typename T>
constexpr ::std::unique_ptr<T> create_unique_ptr(T *_src_ptr) {
  return ::std::unique_ptr<T>{_src_ptr};
}

} // namespace _dev
} // namespace fag

#endif // FAG_DEV_SMARTPTRS_HPP
