// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_VULKANRENDERER_HPP
#define FAG_ENGINE_VULKANRENDERER_HPP

#include <functional>
extern "C" {
#include "fpxlib3d/include/vk.h"
}

#include "./Renderer.hpp"

#include <cstddef>
#include <limits>
#include <unordered_map>
#include <vector>

namespace fag {

class VulkanRenderer : public Renderer {

public:
  static VulkanRenderer *get_singleton(void);
  static void destroy_singleton(void);

public:
  enum PipelineIndex : size_t {
    Default3D = std::numeric_limits<size_t>::max(),
    Default2D = std::numeric_limits<size_t>::max() - 1,
  };

  struct Shader : public Renderer::Shader {};

  struct Shape : public Renderer::Shape {
  public:
    Fpx3d_Vk_Shape vulkanShape;
  };

public:
  void render_frame(void);
  Backend get_backend(void) const;

public:
  void select_pipeline(size_t idx);
  void set_shapes(const std::vector<VulkanRenderer::Shape *> &);

private:
  static VulkanRenderer *m_Singleton;

  struct PipelineLayoutCreator {
    std::vector<Fpx3d_Vk_DescriptorSetLayout> descriptorSetLayouts;
  };

private:
  VulkanRenderer(void);
  ~VulkanRenderer(void);

  void _vulkan_setup(void);
  void _glfw_setup(void);
  void _gpu_setup(void);

  void _create_default3d_pipeline_layout(void);
  void _create_default2d_pipeline_layout(void);

  void _create_pipeline_layouts(void);
  void _create_base_pipelines(void);

private:
  Fpx3d_Vk_Context m_VulkanContext;
  Fpx3d_Wnd_Context m_WindowContext;

  Fpx3d_Vk_PipelineLayout m_Default2DPipelineLayout;
  Fpx3d_Vk_PipelineLayout m_Default3DPipelineLayout;

  std::vector<Fpx3d_Vk_Pipeline *> m_Pipelines;
  Fpx3d_Vk_Pipeline *m_SelectedPipeline = nullptr;
};

} // namespace fag

#endif // FAG_ENGINE_VULKANRENDERER_HPP
