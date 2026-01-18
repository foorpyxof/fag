// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_SHADER_HPP
#define FAG_CORE_SHADER_HPP

#include "./BaseObject.hpp"

namespace fag {

enum class ShaderStage { Vertex, Geometry, Fragment, PixelShading = Fragment };

class Shader : public BaseObject {
public:
  virtual ~Shader(void);

protected:
  Shader(void);
};

} // namespace fag

#endif // FAG_CORE_SHADER_HPP
