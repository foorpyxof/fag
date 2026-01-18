// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Vulkan/Renderer.hpp"
#include "dev/allocators.hpp"
#include "dev/smartptrs.hpp"

#include "debug.h"
#include "fpxlib3d/include/vk/typedefs.h"
#include "macros.hpp"

#include "core/Vulkan/Mesh.hpp"

namespace fag {
namespace Vulkan {

std::shared_ptr<fag::Mesh> Mesh::clone(void) {
  UNUSED(m_VulkanShapeBuffer);

  Mesh *cloned_mesh = nullptr;
  FAG_HEAP_CONSTRUCT(Mesh, cloned_mesh, (*this));

  std::shared_ptr<fag::Mesh> ptr = fag::_dev::create_shared_ptr(cloned_mesh);

  FAG_TODO("create a function within Fpx3d_Vk for cloning a shapebuffer");
  // copy m_VulkanShapeBuffer (of type Fpx3d_Vk_Shapebuffer)
  // (create a function within Fpx3d_Vk that does this)

  // fpx3d_vk_clone_shapebuffer()

  return ptr;
}

const Fpx3d_Vk_ShapeBuffer &Mesh::get_vulkan_shapebuffer(void) const {
  return m_VulkanShapeBuffer;
}

} // namespace Vulkan
} // namespace fag
