#ifndef FAG_ENGINE_BASIS_HPP
#define FAG_ENGINE_BASIS_HPP

#include "./Vectors.hpp"

namespace fag {

struct Basis2D {
public:
  Basis2D();

public:
  Vector2 x, y;
};

struct Basis3D {
public:
  Basis3D();

public:
  Vector3 x, y, z;
};

} // namespace fag

#endif // FAG_ENGINE_BASIS_HPP
