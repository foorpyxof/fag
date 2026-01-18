// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_DRAWABLE_HPP
#define FAG_CORE_DRAWABLE_HPP

#include <memory>

namespace fag {

class Mesh;
class MeshInstance;

class Drawable {
public:
  std::weak_ptr<const MeshInstance> get_meshinstance(void) const;
  std::weak_ptr<const Mesh> get_mesh(void) const;
  void set_mesh(const std::weak_ptr<const Mesh> &);
  void make_mesh_unique(void);

private:
  std::shared_ptr<MeshInstance> m_MeshInstance;
};

} // namespace fag

#endif // FAG_CORE_DRAWABLE_HPP
