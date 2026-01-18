// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_ENTITY3D_HPP
#define FAG_CORE_ENTITY3D_HPP

#include "./Drawable.hpp"
#include "./Entity.hpp"
#include "./Transforms.hpp"
#include "./Vectors.hpp"

namespace fag {

class Entity3D : public Entity, public Drawable {
public:
  virtual void update(void);
  virtual void fixed_update(void);

public:
  const Transform3D &get_transform(void) const;
  void set_transform(Transform3D &);

public:
  Entity3D(void);
  virtual ~Entity3D(void);

private:
  Transform3D m_Transform;
  bool m_TransformHasChanged;
};

} // namespace fag

#endif // FAG_CORE_ENTITY3D_HPP
