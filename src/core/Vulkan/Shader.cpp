// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "error/Generic.hpp"
#include "os/File.hpp"

#include "macros.hpp"

extern "C" {
#include "fpxlib3d/include/vk/shaders.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include <sstream>

extern "C" {
#include <sys/stat.h>
}

#include "core/Vulkan/Shader.hpp"

namespace fag {
namespace Vulkan {

Shader::Shader(const OS::FileBuffer &shader_file, ShaderStage stage) {
  UNUSED(shader_file);

  Fpx3d_Vk_E_ShaderStage shaderstage_converted = SHADER_STAGE_INVALID;
  UNUSED(shaderstage_converted);

  // turn the default 'fag::ShaderStage' value into
  // a value that's valid for Fpx3d_Vk
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

  /*
   * TODO: create function `fpx3d_vk_read_spirv_data` in Fpx3d_Vk
   */
  // m_SpirvData = fpx3d_vk_read_spirv_file(path, shaderstage_converted);

  if (0 == m_SpirvData.filesize)
    throw fag::Error::Generic("an error occured while loading Vulkan shader");

  m_ShaderStage = stage;
}
Shader::~Shader(void) { fpx3d_vk_destroy_spirv_file(&m_SpirvData); }

} // namespace Vulkan
} // namespace fag
