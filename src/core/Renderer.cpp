// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Renderer.hpp"

namespace fag {

// XXX: change this when there's more default render contexts!
#define FAG_BASE_RENDER_CONTEXT_COUNT 2

size_t Renderer::_convert_render_context_index(size_t idx) {
  switch (idx) {
  case Renderer::RenderContextIndex::Default3D:
  case Renderer::RenderContextIndex::Default2D:
    return std::numeric_limits<size_t>::max() - idx;

  default:
    return idx + FAG_BASE_RENDER_CONTEXT_COUNT;
  }
}

Renderer::Renderer(void) {}
Renderer::~Renderer(void) {}

Renderer::Mesh::~Mesh(void) {}

} // namespace fag
