// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_VULKANRENDERER_HPP
#define FAG_ENGINE_VULKANRENDERER_HPP

#include "./Renderer.hpp"
#include <vulkan/vulkan_core.h>

extern "C" {
#include "fpxlib3d/include/vk/context.h"
#include "fpxlib3d/include/vk/pipeline.h"
#include "fpxlib3d/include/vk/shape.h"
#include "fpxlib3d/include/vk/swapchain.h"

#include "fpxlib3d/include/vk/typedefs.h"
#include "fpxlib3d/include/window/window.h"
}

#include <array>
#include <cstddef>
#include <limits>
#include <vector>

using std::array;
using std::vector;

namespace fag {

class VulkanRenderer : public Renderer {
public:
  enum PipelineIndex : size_t {
    Default3D = std::numeric_limits<size_t>::max(),
    Default2D = std::numeric_limits<size_t>::max() - 1,
  };

  struct Shape : public Renderer::Shape {
  public:
    Fpx3d_Vk_Shape vulkanShape;
  };

public:
  VulkanRenderer(void);
  ~VulkanRenderer(void);

  void render_frame(void);
  Backend get_backend(void) const;

public:
  void select_pipeline(size_t idx);
  void set_shapes(const vector<VulkanRenderer::Shape *> &);

private:
  Fpx3d_Vk_SwapchainRequirements m_SwapchainRequirements;

  VkSurfaceFormatKHR m_VulkanSurfaceFormats[1] = {
      {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};
  VkPresentModeKHR m_VulkanPresentModes[1] = {VK_PRESENT_MODE_FIFO_KHR};

  Fpx3d_Vk_Context m_VulkanContext;
  Fpx3d_Wnd_Context m_WindowContext;

  vector<Fpx3d_Vk_Pipeline *> m_Pipelines;
  Fpx3d_Vk_Pipeline *m_SelectedPipeline = nullptr;
};

} // namespace fag

#endif // FAG_ENGINE_VULKANRENDERER_HPP
