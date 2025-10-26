// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Entity3D.hpp"
#include "core/Entity.hpp"
#include "core/Transforms.hpp"

namespace fag {

void Entity3D::update(void) {}
void Entity3D::fixed_update(void) {}

const Transform3D &Entity3D::get_transform(void) const { return m_Transform; }
void Entity3D::set_transform(Transform3D &new_transform) {
  m_Transform = new_transform;
  m_TransformHasChanged = true;
}

Entity3D::Entity3D(void) : m_Transform(), m_TransformHasChanged(true) {}
Entity3D::~Entity3D(void) {}

} // namespace fag
