#ifndef FAG_CORE_DRAWABLE_HPP
#define FAG_CORE_DRAWABLE_HPP

#include <memory>

namespace fag {

class Mesh;

class Drawable {
public:
  std::weak_ptr<Mesh> get_mesh(void) const;
  void swap_mesh(std::shared_ptr<Mesh> &);
  void make_mesh_unique(void);

private:
  std::shared_ptr<Mesh> m_Mesh;
};

} // namespace fag

#endif // FAG_CORE_DRAWABLE_HPP
