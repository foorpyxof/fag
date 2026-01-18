extern "C" {
#include "fpxlib3d/include/vk/shape.h"
}

#include "core/Globals.hpp"
#include "core/Vulkan/Mesh.hpp"
#include "core/Vulkan/Renderer.hpp"
#include "error/Generic.hpp"

#include "core/Vulkan/MeshInstance.hpp"

namespace fag {
namespace Vulkan {

MeshInstance::MeshInstance(void) : m_VulkanShape{} {}
MeshInstance::MeshInstance(const std::weak_ptr<const fag::Mesh> &base_mesh) {
  std::shared_ptr<const fag::Mesh> shr_ptr = base_mesh.lock();
  const fag::Vulkan::Mesh *ptr;
  if (nullptr ==
      (ptr = dynamic_cast<const fag::Vulkan::Mesh *>(shr_ptr.get()))) {
  }

  m_VulkanShape = fpx3d_vk_create_shape(&(ptr->get_vulkan_shapebuffer()));
}

MeshInstance::~MeshInstance(void) { g_Renderer->destroy_meshinstance(*this); }

} // namespace Vulkan
} // namespace fag
