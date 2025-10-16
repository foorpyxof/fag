// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_RENDERER_HPP
#define FAG_CORE_RENDERER_HPP

#include <limits>
#include <memory>
#include <string>

namespace fag {

class Renderer {
public:
  enum RenderContextIndex : size_t;
  class Shader;
  enum class ShaderStage;
  class Mesh;

public:
  template <class T> constexpr bool is() const {
    return (nullptr != dynamic_cast<const T *>(this));
  }

public:
  virtual void render_frame(void) = 0;
  virtual std::weak_ptr<Shader> create_shader(std::string &resource_path,
                                              ShaderStage shader_stage) = 0;

  // Render contexts:
  // for the Vulkan renderer, these are the pipelines
  //
  // Universally, all of the renderers MUST have at least two default
  // render contexts, namely:
  // - Default 3D
  // - Default 2D
  virtual void select_render_context(size_t idx) = 0;
  // virtual void /* return datatype ? */
  // create_render_context(void /* creation data datatype? */) = 0;

  Renderer() {};
  virtual ~Renderer(void) {};

protected:
  size_t _convert_render_context_index(size_t idx);

protected:
  static const size_t BASE_RENDER_CONTEXT_COUNT = 2;

public:
  enum RenderContextIndex : size_t {
    Default3D = std::numeric_limits<size_t>::max(),
    Default2D = std::numeric_limits<size_t>::max() - 1,
  };

  class Shader {};
  enum class ShaderStage {
    Vertex,
    Geometry,
    Fragment,
    PixelShading = Fragment
  };

  class Mesh {
  public:
    virtual Mesh *clone(void) = 0;

    virtual ~Mesh() {};
  };
};

} // namespace fag

#endif // FAG_CORE_RENDERER_HPP
