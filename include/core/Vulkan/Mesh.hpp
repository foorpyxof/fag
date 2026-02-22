// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_MESH_HPP
#define FAG_CORE_VULKAN_MESH_HPP

#include "../Mesh.hpp"
#include "core/Renderer.hpp"
#include "fpxlib3d/include/vk/typedefs.h"

extern "C" {
#include "../../../modules/fpxlib3d/include/vk/shape.h"
}

namespace fag {
namespace Vulkan {

class Mesh : public fag::Mesh {
  friend class Renderer;

public:
  std::shared_ptr<fag::Mesh> clone(void) override;

public:
  const Fpx3d_Vk_ShapeBuffer &get_vulkan_shapebuffer(void) const;

public:
  virtual ~Mesh(void);

private:
  Mesh(const Fpx3d_Vk_ShapeBuffer &);

private:
  Fpx3d_Vk_ShapeBuffer m_VulkanShapeBuffer;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_MESH_HPP
