#include "core/Drawable.hpp"
#include "macros.hpp"

#include <memory>

namespace fag {

std::weak_ptr<Mesh> Drawable::get_mesh(void) const { return m_Mesh; }

void Drawable::swap_mesh(std::shared_ptr<Mesh> &new_mesh) {
  m_Mesh = std::shared_ptr<Mesh>(new_mesh);
}

void Drawable::make_mesh_unique(void) {
  // std::shared_ptr<Mesh> new_mesh = std::shared_ptr
}

} // namespace fag
