// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_MESH_HPP
#define FAG_CORE_VULKAN_MESH_HPP

#include "../Mesh.hpp"

extern "C" {
#include "../../../modules/fpxlib3d/include/vk/shape.h"
}

namespace fag {
namespace Vulkan {

class Mesh : fag::Mesh {
public:
  fag::Mesh *clone(void);

private:
  Fpx3d_Vk_ShapeBuffer m_VulkanShapeBuffer;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_MESH_HPP
