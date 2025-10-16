// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_TRANSFORMS_HPP
#define FAG_CORE_TRANSFORMS_HPP

#include "./Basis.hpp"
#include "./Vectors.hpp"

namespace fag {

struct Transform2D {
public:
  Transform2D() = default;

public:
  Vector2 origin;
  Basis2D basis;
};

struct Transform3D {
public:
  Transform3D() = default;

public:
  Vector3 origin;
  Basis3D basis;
};

} // namespace fag

#endif // FAG_CORE_TRANSFORMS_HPP
