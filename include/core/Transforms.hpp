// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_TRANSFORMS_HPP
#define FAG_CORE_TRANSFORMS_HPP

#include "./Basis.hpp"
#include "./Vectors.hpp"

namespace fag {

struct Transform2D {
  Vector2 origin;
  Basis2D basis;
};

struct Transform3D {
  Vector3 origin;
  Basis3D basis;
};

} // namespace fag

#endif // FAG_CORE_TRANSFORMS_HPP
