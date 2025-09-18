// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_ENGINE_HPP
#define FAG_ENGINE_ENGINE_HPP

#include "./Renderer.hpp"

#include <cstddef>

namespace fag {

class SceneManager;
class Scene;

class Engine {
public:
  static Engine *get_singleton(void);
  static void destroy_singleton(void);

public:
  static Renderer::Backend renderBackend;

public:
  int start(void);
  size_t add_scene(Scene &);

  // tell the engine to finish up and then de-init
  void raise_stop_condition(void);

  // forcefully de-init the engine
  void stop(void);

private:
  static Engine *m_Singleton;

private:
  Engine(void);
  ~Engine(void);

  void _renderer_setup(void);
  void _teardown(void);

private:
  bool m_ShouldStop;

  Renderer *m_Renderer;
  SceneManager *m_SceneManager;
};

} // namespace fag

#endif // FAG_ENGINE_ENGINE_HPP
