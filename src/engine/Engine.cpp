#include "engine/Engine.hpp"
#include "engine/Renderer.hpp"
#include "engine/SceneManager.hpp"
#include "engine/VulkanRenderer.hpp"

extern "C" {
#include "macros.h"
}

#include <cstdlib>
#include <stdexcept>

#define DEFAULT_RENDER_BACKEND Vulkan

namespace fag {

/* static initializations */
Engine *Engine::m_Singleton = nullptr;
Renderer::Backend Engine::renderBackend =
    Renderer::Backend::DEFAULT_RENDER_BACKEND;

/* public static methods */
Engine *Engine::get_singleton(void) {
  if (nullptr == Engine::m_Singleton)
    Engine::m_Singleton = new Engine;

  return Engine::m_Singleton;
}

void Engine::destroy_singleton(void) {
  if (nullptr == Engine::m_Singleton)
    return;

  delete Engine::m_Singleton;
  Engine::m_Singleton = nullptr;
}

/* public instance methods */
int Engine::start(void) {
  _renderer_setup();

  /*
   * Engine startup code goes here!
   */

  FAG_DEBUG("Entering engine loop!");
  while (!m_ShouldStop) {
    /*
     * Engine loop code goes here!
     */
  }

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
      m_SceneManager(new SceneManager) {
  switch (Engine::renderBackend) {
  case Renderer::Backend::Vulkan:
    m_Renderer = new VulkanRenderer;
  }
}
Engine::~Engine(void) {}

void Engine::_renderer_setup(void) {
  NULL_CHECK(m_Renderer,
             FAG_ERROR("Tried to set up renderer, but it is 'nullptr'");
             throw std::runtime_error("renderer pointer invalid"));
  m_Renderer->initialize();
}

void Engine::_teardown(void) {
  FAG_DEBUG("Destroying F.A.G. engine");
  FAG_TODO("Implement 'Engine::_teardown(void)'");
}

} // namespace fag
