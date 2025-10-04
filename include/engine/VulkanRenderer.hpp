// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_VULKANRENDERER_HPP
#define FAG_ENGINE_VULKANRENDERER_HPP

#include "./Renderer.hpp"

extern "C" {
#include "fpxlib3d/include/vk.h"
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
  void _vulkan_setup();
  void _glfw_setup();
  void _gpu_setup();

private:
  Fpx3d_Vk_Context m_VulkanContext;
  Fpx3d_Wnd_Context m_WindowContext;

  vector<Fpx3d_Vk_Pipeline *> m_Pipelines;
  Fpx3d_Vk_Pipeline *m_SelectedPipeline = nullptr;
};

} // namespace fag

#endif // FAG_ENGINE_VULKANRENDERER_HPP
