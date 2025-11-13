// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_VULKAN_SHADER_HPP
#define FAG_CORE_VULKAN_SHADER_HPP

#include "../Shader.hpp"

#include "../../../modules/fpxlib3d/include/vk/shaders.h"

namespace fag {
namespace Vulkan {

class Shader : public fag::Shader {
  friend class Renderer;

public:
  ~Shader(void);

protected:
  Shader(const char *resource_path, ShaderStage);

private:
  ShaderStage m_ShaderStage;
  Fpx3d_Vk_SpirvFile m_SpirvData;
};

} // namespace Vulkan
} // namespace fag

#endif // FAG_CORE_VULKAN_SHADER_HPP
