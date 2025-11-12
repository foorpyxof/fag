// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_MESH_HPP
#define FAG_CORE_MESH_HPP

#include <memory>

namespace fag {

class Mesh {
public:
  virtual std::shared_ptr<Mesh> clone(void) = 0;
  virtual ~Mesh(void);
};

} // namespace fag

#endif // FAG_CORE_MESH_HPP
