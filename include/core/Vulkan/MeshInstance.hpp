// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_VULKAN_MESHINSTANCE_HPP
#define FAG_VULKAN_MESHINSTANCE_HPP

#include "../BaseObject.hpp"

#include "../MeshInstance.hpp"

extern "C" {
#include "../../../modules/fpxlib3d/include/vk/shape.h"
}

namespace fag {
namespace Vulkan {

class Mesh;

class MeshInstance : public fag::MeshInstance {
  friend class Renderer;

public:
  virtual ~MeshInstance(void);

private:
  MeshInstance(const std::weak_ptr<const fag::Vulkan::Mesh> &);

private:
  Fpx3d_Vk_Shape m_VulkanShape;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_VULKAN_MESHINSTANCE_HPP
