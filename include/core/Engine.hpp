// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_ENGINE_HPP
#define FAG_CORE_ENGINE_HPP

#include "./Allocator.hpp"
#include "./Renderer.hpp"
#include "./SceneManager.hpp"

#include <cstddef>

namespace fag {

class SceneManager;
class Scene;

class Engine {
public:
  static Engine *get_singleton(void);
  static void destroy_singleton(void);

  static void set_custom_allocator(Allocator &);
  static const Allocator &get_custom_allocator(void);

public:
  void assign_renderer(Renderer *);
  Renderer *get_renderer(void);

  size_t add_scene(Scene &);

  int start(void);

  // tell the engine to finish up and then de-init
  void raise_stop_condition(void);

  // forcefully de-init the engine
  void stop(void);

public:
  ~Engine(void);

private:
  static Engine *m_Singleton;
  static Allocator m_CustomAllocator;

protected:
  Engine(void);

private:
  void _teardown(void);

private:
  bool m_Running;
  bool m_ShouldStop;

  Renderer *m_Renderer;

  SceneManager *m_SceneManager;
};

} // namespace fag

#endif // FAG_CORE_ENGINE_HPP
