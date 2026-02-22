// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_RENDERER_HPP
#define FAG_CORE_RENDERER_HPP

#include "../core/BaseObject.hpp"
#include "../os/File.hpp"

#include <cstddef>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "../../modules/glm/glm/glm.hpp"

namespace fag {

class Window;

class Entity;

class Mesh;
class MeshInstance;
class Shader;
enum class ShaderStage;

class Texture;

struct RendercontextDescriptor;
struct RendercontextCreationInfo;

struct MeshCreationInfo;
struct MeshInstanceCreationInfo;

struct ViewProjectionMatrices3D;
struct ModelMatrices3D;

class Renderer : public BaseObject {
public:
  virtual void setup_window(const std::weak_ptr<fag::Window> &) const = 0;
  virtual void use_window(const std::weak_ptr<fag::Window> &) = 0;

  virtual void render_frame(void) = 0;

  // Render contexts:
  // e.g.: for the Vulkan renderer, these are the pipelines
  virtual void select_render_context(size_t idx) = 0;

  // There is also the ability to create new, custom render contexts
  virtual size_t
  create_render_context(const fag::RendercontextCreationInfo &) = 0;

  // sets the array of entities within the render context
  virtual void
  set_entities(const std::vector<std::weak_ptr<fag::Entity>> &) = 0;

public:
  // creation and destruction methods for Renderer-specific objects;
  // not the window, because those are not particularly linked
  // to a specific Renderer implementation
  virtual std::shared_ptr<fag::Mesh>
  create_mesh(const fag::MeshCreationInfo &) = 0;
  virtual std::shared_ptr<fag::MeshInstance>
  create_meshinstance(const fag::MeshInstanceCreationInfo &) = 0;
  virtual std::shared_ptr<fag::Shader>
  create_shader(const fag::OS::FileBuffer &shader_file,
                fag::ShaderStage stage) = 0;

  virtual void destroy_mesh(fag::Mesh &) = 0;
  virtual void destroy_meshinstance(fag::MeshInstance &) = 0;
  virtual void destroy_shader(fag::Shader &) = 0;

public:
  void set_main_window(const std::weak_ptr<fag::Window> &);
  std::weak_ptr<fag::Window> get_main_window(void);
  std::weak_ptr<fag::Window> get_selected_window(void);

public:
  virtual ~Renderer(void);

protected:
  size_t _convert_render_context_index(size_t idx);

  void _set_selected_window(const std::weak_ptr<fag::Window> &);

protected:
  Renderer(void);

private:
  std::weak_ptr<fag::Window> m_MainWindow;
  std::weak_ptr<fag::Window> m_SelectedWindow;
};

struct RendercontextDescriptor {
  enum class Type {
    Uniform,
    Sampler2D,
    Texture = Sampler2D,
  } type;
  size_t size, count;

  RendercontextDescriptor(size_t uniform_size, size_t uniform_count);
  RendercontextDescriptor(size_t texture_size, size_t texture_count,
                          const fag::Texture &texture);
};

struct RendercontextCreationInfo {
  enum class Type { Rendercontext3D = 0, Rendercontext2D = 1 } type;

  fag::Shader **shaderArray;
  size_t shaderCount;

  std::vector<RendercontextDescriptor> contextDescriptors, objectDescriptors;
  // TODO: attributes, bindings, blah blah blah
};

struct MeshCreationInfo {
  void *vertexData;
  size_t vertexCount;
  size_t stride;

  uint32_t *indices;
  size_t indexCount;
};
struct MeshInstanceCreationInfo {
  std::weak_ptr<const Mesh> meshPointer;
};

struct ViewProjectionMatrices3D {
  glm::mat4 view, projection;
};
struct ModelMatrices3D {
  glm::mat4 view, projection;
};

} // namespace fag

#endif // FAG_CORE_RENDERER_HPP
