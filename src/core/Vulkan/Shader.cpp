// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "debug.h"
#include "error/Generic.hpp"
#include "error/Internal.hpp"
#include "os/File.hpp"

#include "macros.hpp"

extern "C" {
#include "fpxlib3d/include/vk/shaders.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include <cstring>
#include <sstream>

extern "C" {
#include <sys/stat.h>
}

#include "core/Vulkan/Shader.hpp"

namespace fag {
namespace Vulkan {

Shader::Shader(const Shader &other)
    : m_ShaderStage(other.m_ShaderStage), m_SpirvData(other.m_SpirvData) {
  // deep copy of SPIR-V data
  m_SpirvData = fpx3d_vk_read_spirv_data(
      m_SpirvData.buffer, m_SpirvData.filesize, m_SpirvData.stage);

  if (nullptr == m_SpirvData.buffer) {
    FAG_ERROR(fag::Vulkan::Shader,
              "failed to deep-copy fpxlib3d SPIR-V module");

    std::ostringstream errmsg;
    errmsg << "error while trying to copy a shader.";
    throw fag::Error::Internal(errmsg.str().c_str(), __FILE__, __LINE__);
  }
}
Shader::Shader(const OS::FileBuffer &shader_file, ShaderStage stage) {
  Fpx3d_Vk_E_ShaderStage shaderstage_converted = SHADER_STAGE_INVALID;

  FAG_TODO("make Vulkan shader only constructible using Vulkan::Renderer");

  // turn the 'fag::ShaderStage' value into
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

  uint8_t *spirv_bytes = new uint8_t[shader_file.get_size()];
  shader_file.read(spirv_bytes, FAG_FILE_POSITION_END);

  m_SpirvData = fpx3d_vk_read_spirv_data(spirv_bytes, shader_file.get_size(),
                                         shaderstage_converted);

  delete[] spirv_bytes;

  if (0 == m_SpirvData.filesize)
    throw fag::Error::Generic("an error occured while loading Vulkan shader");

  FAG_DEBUG(fag::Vulkan::Shader, "Shader at '%s' loaded successfully",
            shader_file.get_file_path().c_str());

  m_ShaderStage = stage;
}

Shader::~Shader(void) { fpx3d_vk_destroy_spirv_file(&m_SpirvData); }

} // namespace Vulkan
} // namespace fag
