#include "engine/Engine.hpp"
#include "engine/Renderer.hpp"
#include "engine/SceneManager.hpp"

extern "C" {
#include "macros.h"
}

#include <cstdlib>

namespace fag {

/* static initializations */
Engine *Engine::m_Singleton = nullptr;

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
int Engine::start() {
  /*
   * Engine code goes here!
   */
  _renderer_setup();

  return EXIT_SUCCESS;
}

size_t Engine::add_scene(Scene &to_append) {
  return m_SceneManager->append_scene(to_append);
}

void Engine::raise_stop_condition() { m_ShouldStop = true; }

void Engine::stop() {}

/* private static methods */

/* private methods */
Engine::Engine()
    : m_ShouldStop(false), m_Renderer(new Renderer),
      m_SceneManager(new SceneManager) {}
Engine::~Engine() {}

int Engine::_renderer_setup() {
  NULL_CHECK(m_Renderer, return EXIT_FAILURE);
  return m_Renderer->initialize();
}

} // namespace fag
