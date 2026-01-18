// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Renderer.hpp"
#include "core/SceneManager.hpp"
#include "core/Vulkan/Renderer.hpp"

#include "error/Generic.hpp"

#include "dev/allocators.hpp"

#include <sstream>

#include "core/Engine.hpp"

extern "C" {
#include "macros.hpp"
}

#include <cstdlib>
#include <stdexcept>

#define DEFAULT_RENDER_BACKEND Vulkan

namespace fag {

Engine *Engine::s_Singleton = nullptr;
Allocator Engine::s_CustomAllocator = {};

Engine *Engine::get_singleton(void) {
  if (nullptr == Engine::s_Singleton) {
    Engine::s_Singleton = new Engine;
  }

  return Engine::s_Singleton;
}

void Engine::destroy_singleton(void) {
  if (nullptr == Engine::s_Singleton)
    return;

  delete Engine::s_Singleton;
  Engine::s_Singleton = nullptr;
}

const Allocator &Engine::get_custom_allocator(void) {
  return s_CustomAllocator;
}

void Engine::set_custom_allocator(Allocator &allocator) {
  if (!s_Singleton)
    return;

  if (s_Singleton->m_Running)
    throw Error::Generic(
        "cannot assign a custom allocator while the engine is running");

  if (!allocator.alloc_func || !allocator.free_func)
    return;

  s_CustomAllocator = allocator;
}

void Engine::assign_renderer(Renderer *r) {
  if (m_Running)
    throw Error::Generic(
        "cannot assign a renderer while the engine is running");

  m_Renderer = r;
}
Renderer *Engine::get_renderer(void) { return m_Renderer; }

size_t Engine::add_scene(Scene &to_append) {
  return m_SceneManager->append_scene(to_append);
}

int Engine::start(void) {
  /*
   * Engine startup code goes here!
   */

  if (nullptr == m_Renderer)
    throw Error::Generic(
        // throw std::runtime_error(
        "No renderer was assigned to the engine before startup");

  m_Running = true;

  FAG_DEBUG(fag::Engine, "Entering engine loop!");
  // while (!m_ShouldStop) {
  /*
   * Engine loop code goes here!
   */

  // process all entities

  // render the frame once we're done
  m_Renderer->render_frame();
  // }

  return 0;
}

void Engine::raise_stop_condition(void) { m_ShouldStop = true; }
void Engine::stop(void) { _teardown(); }

void Engine::_teardown(void) {
  m_Running = false;
  FAG_DEBUG(fag::Engine, "Destroying F.A.G. engine");

  delete m_SceneManager;
}

Engine::Engine() : m_Running(false), m_ShouldStop(false), m_Renderer(nullptr) {
  m_SceneManager = new SceneManager;
}
Engine::~Engine(void) { _teardown(); }

} // namespace fag
