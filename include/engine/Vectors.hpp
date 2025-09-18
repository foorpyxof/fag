// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_VECTORS_HPP
#define FAG_ENGINE_VECTORS_HPP

namespace fag {

struct Vector2 {
public:
  Vector2() : x(0), y(0) {}
  Vector2(float init) : x(init), y(init) {}
  Vector2(float val1, float val2) : x(val1), y(val2) {}

public:
  float x, y;
};

struct Vector3 {
public:
  Vector3() : x(0), y(0), z(0) {}
  Vector3(float init) : x(init), y(init), z(init) {}
  Vector3(float val1, float val2, float val3) : x(val1), y(val2), z(val3) {}

public:
  union {
    float x, r;
  };
  union {
    float y, g;
  };
  union {
    float z, b;
  };
};

struct Vector4 {
public:
  Vector4() : x(0), y(0), z(0) {}
  Vector4(float init) : x(init), y(init), z(init) {}
  Vector4(float val1, float val2, float val3, float val4)
      : x(val1), y(val2), z(val3), w(val4) {}

public:
  union {
    float x, r;
  };
  union {
    float y, g;
  };
  union {
    float z, b;
  };
  union {
    float w, a;
  };
};

} // namespace fag

#endif // FAG_ENGINE_VECTORS_HPP
