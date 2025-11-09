#ifndef FAG_CORE_VULKANMESH_HPP
#define FAG_CORE_VULKANMESH_HPP

#include "./Mesh.hpp"

#include "../../modules/fpxlib3d/include/vk/shape.h"

namespace fag {
namespace Vulkan {

class Mesh : fag::Mesh {
public:
  fag::Mesh *clone(void);

private:
  Fpx3d_Vk_ShapeBuffer m_VulkanShapeBuffer;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKANMESH_HPP
