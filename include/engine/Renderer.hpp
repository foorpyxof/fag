// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_RENDERER_HPP
#define FAG_ENGINE_RENDERER_HPP

namespace fag {

class Renderer {
public:
  enum class Backend { Vulkan };

  struct Shader {};
  struct Shape {};

public:
  virtual ~Renderer(void) {};

  virtual void render_frame(void) = 0;
  virtual Backend get_backend(void) const = 0;
};

} // namespace fag

#endif // FAG_ENGINE_RENDERER_HPP
