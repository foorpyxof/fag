// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_WINDOW_HPP
#define FAG_CORE_VULKAN_WINDOW_HPP

#include "../Window.hpp"

// clang-format off
#define VK_NO_PROTOTYPES
#include "../../../modules/fpxlib3d/modules/Vulkan-Headers/include/vulkan/vulkan.h"
// clang-format on

#include <string>

namespace fag {
namespace Vulkan {

class Renderer;

class Window : public virtual fag::Window {
public:
  virtual void create_vulkan_surface(VkInstance) = 0;
  VkSurfaceKHR get_vulkan_surface(void) const;

public:
  virtual ~Window(void);

protected:
  void _set_vulkan_instance(VkInstance &);
  void _set_vulkan_surface(VkSurfaceKHR &);

private:
  VkInstance m_VulkanInstance;
  VkSurfaceKHR m_VulkanSurface;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_WINDOW_HPP
