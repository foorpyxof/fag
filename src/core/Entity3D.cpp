// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Entity3D.hpp"
#include "core/Entity.hpp"
#include "core/Transforms.hpp"

namespace fag {

Entity3D::Entity3D() : m_Transform(), m_TransformHasChanged(true) {
  _set_basetype(Entity::Basetype::Entity3D);
}
Entity3D::~Entity3D() {}

void Entity3D::update() {}
void Entity3D::fixed_update() {}

Transform3D Entity3D::get_transform() const { return m_Transform; }
void Entity3D::set_transform(Transform3D &new_transform) {
  m_Transform = new_transform;
}

} // namespace fag
