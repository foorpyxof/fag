#include "core/VulkanMesh.hpp"

#include "dev/allocators.hpp"

namespace fag {
namespace Vulkan {

fag::Mesh *Mesh::clone(void) {
  Mesh *cloned_mesh;
  FAG_HEAP_CONSTRUCT(Mesh, cloned_mesh, ());

  cloned_mesh->m_VulkanShapeBuffer = m_VulkanShapeBuffer;

  return cloned_mesh;
}

} // namespace Vulkan
} // namespace fag
