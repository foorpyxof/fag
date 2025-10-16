// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKANRENDERER_HPP
#define FAG_CORE_VULKANRENDERER_HPP

extern "C" {
#include "fpxlib3d/include/vk.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include "./Renderer.hpp"

#include <cstddef>
#include <vector>

namespace fag {

class VulkanRenderer : public Renderer {
public:
  class Shader;
  class Mesh;

public:
  static VulkanRenderer *get_singleton(void);
  static void destroy_singleton(void);

public:
  void render_frame(void);
  std::weak_ptr<Renderer::Shader> create_shader(std::string &resource_path,
                                                ShaderStage stage_flag);

  void select_render_context(size_t idx);

public:
  void set_shapes(const std::vector<VulkanRenderer::Mesh *> &);

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

public:
  class Shader : Renderer::Shader {};
  class Mesh : Renderer::Mesh {
  public:
    Renderer::Mesh *clone(void);

  private:
    Fpx3d_Vk_ShapeBuffer m_VulkanShapeBuffer;
  };
};

} // namespace fag

#endif // FAG_CORE_VULKANRENDERER_HPP
