// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Vulkan/Shader.hpp"
#include "error/Generic.hpp"

extern "C" {
#include "fpxlib3d/include/vk/shaders.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include <sstream>

extern "C" {
#include <sys/stat.h>
}

namespace fag {
namespace Vulkan {

Shader::Shader(const char *path, ShaderStage stage) {
  {
    struct stat filestat;
    if (0 != stat(path, &filestat)) {
      std::ostringstream err_msg;
      err_msg << "Vulkan shader file at '" << path << "' not found.";

      throw fag::Error::Generic(err_msg.str().c_str());
    }
  }

  Fpx3d_Vk_E_ShaderStage shaderstage_converted = SHADER_STAGE_INVALID;

  switch (stage) {
  case ShaderStage::Vertex:
    shaderstage_converted = SHADER_STAGE_VERTEX;
    break;
  case ShaderStage::Geometry:
    shaderstage_converted = SHADER_STAGE_GEOMETRY;
    break;
  case ShaderStage::Fragment:
    shaderstage_converted = SHADER_STAGE_FRAGMENT;
    break;

  default:
    // invalid shader stage
    throw fag::Error::Generic(
        "invalid shader stage passed to Vulkan shader creation");
    break;
  }

  m_SpirvData = fpx3d_vk_read_spirv_file(path, shaderstage_converted);

  if (0 == m_SpirvData.filesize)
    throw fag::Error::Generic("an error occured while loading Vulkan shader");

  m_ShaderStage = stage;
}
Shader::~Shader(void) { fpx3d_vk_destroy_spirv_file(&m_SpirvData); }

} // namespace Vulkan
} // namespace fag
