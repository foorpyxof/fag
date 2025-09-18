// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_VULKANRENDERER_HPP
#define FAG_ENGINE_VULKANRENDERER_HPP

#include "./Renderer.hpp"

#include <cstddef>
#include <vector>

namespace fag {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(void);
  ~VulkanRenderer(void);

  void initialize(void);
  void render_frame(void);

  Backend get_backend(void);

  void select_pipeline(size_t idx);
  void add_shapes(std::vector<Renderer::Shape> &);
};

} // namespace fag

#endif // FAG_ENGINE_VULKANRENDERER_HPP
