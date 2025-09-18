// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_ENTITY3D_HPP
#define FAG_ENGINE_ENTITY3D_HPP

#include "./Entity.hpp"
#include "./Transforms.hpp"
#include "./Vectors.hpp"

namespace fag {

class Entity3D : public Entity {
public:
  Entity3D();
  virtual ~Entity3D();

  virtual void update();
  virtual void fixed_update();

  Transform3D get_transform() const;
  void set_transform(Transform3D &);

private:
  friend class Renderer;

  Transform3D m_Transform;
  bool m_TransformHasChanged;
};

} // namespace fag

#endif // FAG_ENGINE_ENTITY3D_HPP
