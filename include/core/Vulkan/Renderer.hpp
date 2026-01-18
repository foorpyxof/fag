// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_RENDERER_HPP
#define FAG_CORE_VULKAN_RENDERER_HPP

#include "../Renderer.hpp"

#include "../Entity.hpp"
#include "./MeshInstance.hpp"
#include "./Shader.hpp"
#include <memory>

extern "C" {
#include "../../../modules/fpxlib3d/include/vk.h"
}

#include <cstddef>
#include <vector>

namespace fag {
namespace Vulkan {

class Renderer : public fag::Renderer {
public:
  void render_frame(void);

  void select_render_context(size_t idx);
  size_t create_render_context(const fag::RendercontextCreationInfo &);

  void set_entities(const std::vector<std::weak_ptr<fag::Entity>> &);

  std::shared_ptr<fag::Mesh>
      create_mesh(/* specify mesh creation requirements */);
  std::shared_ptr<fag::MeshInstance>
  create_meshinstance(fag::MeshInstanceCreationInfo &);

  void destroy_mesh(fag::Mesh &);
  void destroy_meshinstance(fag::MeshInstance &);

public:
  Renderer(void);
  ~Renderer(void);

private:
  void _vulkan_setup(void);
  void _glfw_setup(void);
  void _gpu_setup(void);

  void _prepare_framebuffers(void);

  void _teardown(void);

private:
  Fpx3d_Vk_Context m_VulkanContext;
  Fpx3d_Wnd_Context m_WindowContext;

  Fpx3d_Vk_LogicalGpu *m_LogicalGpuPtr;

  size_t m_SelectedPipelineIndex = 0;

  struct _pipeline_properties;
  std::vector<struct _pipeline_properties> m_PipelineProperties;

private:
  struct _pipeline_properties {
    fag::RendercontextCreationInfo::Type pipelineType;
  };
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_RENDERER_HPP
