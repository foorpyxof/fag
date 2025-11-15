// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Mesh.hpp"

#include "macros.hpp"

#include "core/Drawable.hpp"

#include <memory>

namespace fag {

std::weak_ptr<Mesh> Drawable::get_mesh(void) const { return m_Mesh; }

void Drawable::set_mesh(std::shared_ptr<Mesh> &new_mesh) {
  m_Mesh = std::shared_ptr<Mesh>(new_mesh);
}

void Drawable::make_mesh_unique(void) { m_Mesh = m_Mesh->clone(); }

} // namespace fag
