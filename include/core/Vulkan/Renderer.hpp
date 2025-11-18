// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_RENDERER_HPP
#define FAG_CORE_VULKAN_RENDERER_HPP

#include "../Renderer.hpp"

#include "./Shader.hpp"
#include "fpxlib3d/include/vk/typedefs.h"
#include "os/File.hpp"

extern "C" {
#include "../../../modules/fpxlib3d/include/vk.h"
}

#include <cstddef>
#include <vector>

namespace fag {
namespace Vulkan {

struct RendercontextCreationInfo;

class Renderer : public fag::Renderer {
public:
  void render_frame(void);

  void select_render_context(size_t idx);
  size_t create_render_context(const fag::RendercontextCreationInfo &);

  void set_shapes(const std::vector<fag::Mesh *> &);

public:
  Renderer(void);
  ~Renderer(void);

private:
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

struct RendercontextCreationInfo : private fag::RendercontextCreationInfo {
  fag::Vulkan::Shader *shaderArray;
  size_t shaderCount;

  Fpx3d_Vk_DescriptorSetBinding balls;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_RENDERER_HPP
