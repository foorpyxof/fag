#ifndef FAG_ENGINE_BASIS_HPP
#define FAG_ENGINE_BASIS_HPP

#include "./Vectors.hpp"

namespace fag {

struct Basis2D {
public:
  Basis2D() : x(1, 0), y(0, 1) {}

public:
  Vector2 x, y;
};

struct Basis3D {
public:
  Basis3D() : x(1, 0, 0), y(0, 1, 0), z(0, 0, 1) {}

public:
  Vector3 x, y, z;
};

} // namespace fag

#endif // FAG_ENGINE_BASIS_HPP
