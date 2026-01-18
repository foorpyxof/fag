// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_MESHINSTANCE_HPP
#define FAG_CORE_MESHINSTANCE_HPP

#include "./BaseObject.hpp"

#include <memory>

namespace fag {

class Mesh;

class MeshInstance : public BaseObject {
public:
  virtual ~MeshInstance(void);

public:
  std::weak_ptr<const fag::Mesh> get_base_mesh();

public:
  MeshInstance(void);
  MeshInstance(const std::weak_ptr<const fag::Mesh> &);

private:
  std::weak_ptr<const fag::Mesh> m_Mesh;
};

} // namespace fag

#endif // FAG_CORE_MESHINSTANCE_HPP
