#include "core/MeshInstance.hpp"

namespace fag {

std::weak_ptr<const fag::Mesh> MeshInstance::get_base_mesh(void) {
  return m_Mesh;
}

MeshInstance::MeshInstance(void) : m_Mesh() {}
MeshInstance::MeshInstance(const std::weak_ptr<const fag::Mesh> &base_mesh)
    : m_Mesh(base_mesh) {}

MeshInstance::~MeshInstance(void) {};

} // namespace fag
