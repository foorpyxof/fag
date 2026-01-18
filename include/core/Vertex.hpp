// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VERTEX_HPP
#define FAG_CORE_VERTEX_HPP

#include "./Vectors.hpp"

namespace fag {

struct Vertex2D {
  Vector2 position;
  Vector3 color;
  Vector2 textureCoordinate;
};

struct Vertex3D {
  Vector3 position;
  Vector3 color;
  Vector2 textureCoordinate;
};

} // namespace fag

#endif // FAG_CORE_VERTEX_HPP
