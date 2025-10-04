// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "engine/VulkanRenderer.hpp"
#include "engine/Renderer.hpp"
#include "fpxlib3d/include/vk/context.h"

extern "C" {
#include "debug.h"
#include "macros.h"

#include "fpxlib3d/include/vk/swapchain.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include <limits>
#include <vulkan/vulkan_core.h>

namespace fag {

// XXX: change this when there's more default pipelines!
#define BASE_PIPELINE_COUNT 2

VulkanRenderer::VulkanRenderer(void) : m_Pipelines(BASE_PIPELINE_COUNT) {
  UNUSED(m_VulkanContext);
  UNUSED(m_VulkanSurfaceFormats);
  UNUSED(m_VulkanPresentModes);

  fpx3d_vk_set_required_presentmodes(&m_SwapchainRequirements,
                                     m_VulkanPresentModes,
                                     ARRAY_SIZE(m_VulkanPresentModes));
  fpx3d_vk_set_required_surfaceformats(&m_SwapchainRequirements,
                                       m_VulkanSurfaceFormats,
                                       ARRAY_SIZE(m_VulkanSurfaceFormats));

  fpx3d_vk_init_context(&m_VulkanContext, &m_WindowContext);
}
VulkanRenderer::~VulkanRenderer(void) {}

IMPLEMENT_THIS(void VulkanRenderer::render_frame(void), );

Renderer::Backend VulkanRenderer::get_backend(void) const {
  return Renderer::Backend::Vulkan;
}

void VulkanRenderer::select_pipeline(size_t idx) {
  size_t real_index;

  switch (idx) {
  case PipelineIndex::Default2D:
  case PipelineIndex::Default3D:
    real_index = std::numeric_limits<size_t>::max() - idx;
    break;

  default:
    real_index = idx + BASE_PIPELINE_COUNT;
    break;
  }

  if (real_index >= m_Pipelines.size())
    return;

  m_SelectedPipeline = m_Pipelines[real_index];
}
IMPLEMENT_THIS(void VulkanRenderer::set_shapes(
                   const std::vector<VulkanRenderer::Shape *> &shapes),
               UNUSED(shapes););

} // namespace fag
