#include "engine/VulkanRenderer.hpp"
#include "engine/Renderer.hpp"

extern "C" {
#include "debug.h"
#include "macros.h"
}

namespace fag {

VulkanRenderer::VulkanRenderer(void) {}
VulkanRenderer::~VulkanRenderer(void) {}

void VulkanRenderer::initialize(void) {
  FAG_TODO("Implement 'VulkanRenderer::initialize(void)'");
}
void VulkanRenderer::render_frame(void) {
  FAG_TODO("Implement 'VulkanRenderer::render_frame(void)'");
}
Renderer::Backend VulkanRenderer::get_backend(void) {
  return Renderer::Backend::Vulkan;
}

void VulkanRenderer::select_pipeline(size_t idx) {
  UNUSED(idx);
  FAG_TODO("Implement 'VulkanRenderer::select_pipeline(void)'");
}
void VulkanRenderer::add_shapes(std::vector<Renderer::Shape> &shapes) {
  UNUSED(shapes);
  FAG_TODO("Implement 'VulkanRenderer::add_shapes(void)'");
}

} // namespace fag
