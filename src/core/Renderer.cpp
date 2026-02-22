// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Renderer.hpp"
#include "core/Window.hpp"
#include "error/Generic.hpp"

#include "debug.h"
#include "macros.hpp"

namespace fag {

Renderer::Renderer(void) {}
Renderer::~Renderer(void) {}

void Renderer::set_main_window(const std::weak_ptr<fag::Window> &win) {
  if (m_MainWindow.lock())
    throw fag::Error::Generic(
        "main_window is already set and cannot be re-assigned");

  m_MainWindow = win;
}

std::weak_ptr<fag::Window> Renderer::get_main_window(void) {
  return m_MainWindow;
}

std::weak_ptr<fag::Window> Renderer::get_selected_window(void) {
  return m_SelectedWindow;
}

// XXX: change this when there's more default render contexts!
// #define FAG_BASE_RENDER_CONTEXT_COUNT 2

size_t Renderer::_convert_render_context_index(size_t idx) {
  //   switch (idx) {
  //   case Renderer::RenderContextIndex::Default3D:
  //   case Renderer::RenderContextIndex::Default2D:
  //     return std::numeric_limits<size_t>::max() - idx;
  //
  //   default:
  //     return idx + FAG_BASE_RENDER_CONTEXT_COUNT;
  //   }
  return idx;
}

void Renderer::_set_selected_window(const std::weak_ptr<fag::Window> &win) {
  m_SelectedWindow = win;
}

RendercontextDescriptor::RendercontextDescriptor(size_t _uniform_size,
                                                 size_t _uniform_count)
    : type(Type::Uniform), size(_uniform_size), count(_uniform_count) {}
RendercontextDescriptor::RendercontextDescriptor(size_t _texture_size,
                                                 size_t _texture_count,
                                                 const fag::Texture &_texture)
    : type(Type::Sampler2D), size(_texture_size), count(_texture_count) {
  UNUSED(_texture);
  FAG_TODO(
      "implement RendercontextDescriptor constructor for Texture descriptors");
}

} // namespace fag
