// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Engine.hpp"
#include "core/Renderer.hpp"
#include "core/SceneManager.hpp"
#include "core/VulkanRenderer.hpp"

#include "error/Generic.hpp"

#include "dev/allocators.hpp"

extern "C" {
#include "macros.hpp"
}

#include <cstdlib>
#include <stdexcept>

#define DEFAULT_RENDER_BACKEND Vulkan

namespace fag {

/* static initializations */
Engine *Engine::m_Singleton = nullptr;
/* public static methods */
Engine *Engine::get_singleton(void) {
  if (nullptr == Engine::m_Singleton)
    Engine::m_Singleton = FAG_HEAP_CONSTRUCT(Engine);

  return Engine::m_Singleton;
}

void Engine::destroy_singleton(void) {
  if (nullptr == Engine::m_Singleton)
    return;

  delete Engine::m_Singleton;
  Engine::m_Singleton = nullptr;
}

/* public instance methods */
void Engine::assign_renderer(Renderer *r) { m_Renderer = r; }

int Engine::start(void) {
  /*
   * Engine startup code goes here!
   */

  if (nullptr == m_Renderer)
    throw Error::Generic(
        // throw std::runtime_error(
        "No renderer was assigned to the engine before startup");

  // FAG_DEBUG("Entering engine loop!");
  // while (!m_ShouldStop) {
  //   /*
  //    * Engine loop code goes here!
  //    */
  // }

  _teardown();

  return 0;
}

size_t Engine::add_scene(Scene &to_append) {
  return m_SceneManager->append_scene(to_append);
}

void Engine::raise_stop_condition(void) { m_ShouldStop = true; }

void Engine::stop(void) { _teardown(); }

/* private static methods */

/* private methods */
Engine::Engine()
    : m_ShouldStop(false), m_Renderer(nullptr),
      m_SceneManager(FAG_HEAP_CONSTRUCT(SceneManager)) {}
Engine::~Engine(void) {}

void Engine::_teardown(void) {
  FAG_DEBUG("Destroying F.A.G. engine");
  FAG_TODO("Implement 'Engine::_teardown(void)'");
}

} // namespace fag
