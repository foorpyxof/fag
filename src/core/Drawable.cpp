// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Globals.hpp"
#include "core/Mesh.hpp"
#include "core/MeshInstance.hpp"
#include "core/Renderer.hpp"

#include "macros.hpp"

#include "core/Drawable.hpp"

#include <memory>

namespace fag {

std::weak_ptr<const MeshInstance> Drawable::get_meshinstance(void) const {
  return m_MeshInstance;
}

std::weak_ptr<const Mesh> Drawable::get_mesh(void) const {
  return m_MeshInstance->get_base_mesh();
}

void Drawable::set_mesh(const std::weak_ptr<const Mesh> &new_mesh) {
  UNUSED(new_mesh);

  if (m_MeshInstance)
    g_Renderer->destroy_meshinstance(*m_MeshInstance);

  fag::MeshInstanceCreationInfo mi_create{new_mesh};
  m_MeshInstance = g_Renderer->create_meshinstance(mi_create);
}

void Drawable::make_mesh_unique(void) {
  /* std::shared_ptr<Mesh> temp_shared = m_Mesh.lock();
  m_Mesh = temp_shared->clone(); */
}

} // namespace fag
