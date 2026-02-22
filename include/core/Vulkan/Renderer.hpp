// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_RENDERER_HPP
#define FAG_CORE_VULKAN_RENDERER_HPP

#include "../Renderer.hpp"

#include "../Entity.hpp"
#include "./MeshInstance.hpp"
#include "./Shader.hpp"
#include "core/Shader.hpp"
#include "os/File.hpp"
#include <memory>

extern "C" {
#include "../../../modules/fpxlib3d/include/vk.h"
}

#include <cstddef>
#include <vector>

namespace fag {
namespace Vulkan {

class Window;

class Renderer : public fag::Renderer {
public:
  void setup_window(const std::weak_ptr<fag::Window> &) const override;
  void use_window(const std::weak_ptr<fag::Window> &) override;

  void render_frame(void) override;

  void select_render_context(size_t idx) override;
  size_t create_render_context(const fag::RendercontextCreationInfo &) override;

  void set_entities(const std::vector<std::weak_ptr<fag::Entity>> &) override;

  std::shared_ptr<fag::Mesh>
  create_mesh(const fag::MeshCreationInfo &) override;
  std::shared_ptr<fag::MeshInstance>
  create_meshinstance(const fag::MeshInstanceCreationInfo &) override;
  std::shared_ptr<fag::Shader>
  create_shader(const fag::OS::FileBuffer &shader_file,
                fag::ShaderStage stage) override;

  void destroy_mesh(fag::Mesh &) override;
  void destroy_meshinstance(fag::MeshInstance &) override;
  void destroy_shader(fag::Shader &) override;

public:
  Renderer(const std::weak_ptr<fag::Vulkan::Window> &);
  ~Renderer(void);

private:
  void _vulkan_setup(const std::shared_ptr<fag::Vulkan::Window> &);
  void _glfw_setup(void);
  void _gpu_setup(void);

  void _prepare_command_buffers(void);
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
