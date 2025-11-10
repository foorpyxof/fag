// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_RENDERER_HPP
#define FAG_CORE_RENDERER_HPP

#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace fag {

class Mesh;
class Shader;

class Renderer {
public:
  enum RenderContextIndex : size_t;
  enum class ShaderStage;

public:
  virtual void render_frame(void) = 0;
  virtual std::weak_ptr<Shader> create_shader(std::string &resource_path,
                                              ShaderStage shader_stage) = 0;

  // Render contexts:
  // e.g.: for the Vulkan renderer, these are the pipelines
  //
  // Universally, all of the renderers MUST have at least two default
  // render contexts, namely:
  // - Default 3D
  // - Default 2D
  virtual void select_render_context(size_t idx) = 0;

  // There is also the ability to create new, custom render contexts
  virtual void create_render_context(void *) = 0;

  virtual void set_shapes(const std::vector<Mesh *> &) = 0;

public:
  template <class T> constexpr bool is() const {
    return (nullptr != dynamic_cast<const T *>(this));
  }

public:
  virtual ~Renderer(void);

protected:
  size_t _convert_render_context_index(size_t idx);

protected:
  Renderer(void);

protected:
  static const size_t BASE_RENDER_CONTEXT_COUNT = 2;

public:
  // nested type definitions
  enum RenderContextIndex : size_t {
    Default3D = std::numeric_limits<size_t>::max(),
    Default2D = std::numeric_limits<size_t>::max() - 1,
  };

  enum class ShaderStage {
    Vertex,
    Geometry,
    Fragment,
    PixelShading = Fragment
  };
};

} // namespace fag

#endif // FAG_CORE_RENDERER_HPP
