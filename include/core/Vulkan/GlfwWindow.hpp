// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_GLFWWINDOW_HPP
#define FAG_CORE_VULKAN_GLFWWINDOW_HPP

#include "../GlfwWindow.hpp"
#include "./Window.hpp"

#include <string>

namespace fag {
namespace Vulkan {

class GlfwWindow : public fag::Vulkan::Window, public fag::GlfwWindow {
public:
  void create_vulkan_surface(VkInstance) override;

public:
  GlfwWindow(void);
  GlfwWindow(int width, int height);

  ~GlfwWindow(void);
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_GLFWWINDOW_HPP
