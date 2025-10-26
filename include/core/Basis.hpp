// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_BASIS_HPP
#define FAG_CORE_BASIS_HPP

#include "./Vectors.hpp"

namespace fag {

struct Basis2D {
  Vector2 x, y;

  Basis2D() : x(1, 0), y(0, 1) {}
};

struct Basis3D {
  Vector3 x, y, z;

  Basis3D() : x(1, 0, 0), y(0, 1, 0), z(0, 0, 1) {}
};

} // namespace fag

#endif // FAG_CORE_BASIS_HPP
