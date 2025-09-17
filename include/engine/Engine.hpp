#ifndef FAG_ENGINE_ENGINE_HPP
#define FAG_ENGINE_ENGINE_HPP

#include <cstddef>

namespace fag {

class Renderer;
class SceneManager;
class Scene;

class Engine {
public:
  static Engine *get_singleton(void);
  static void destroy_singleton(void);

public:
  int start(void);
  size_t add_scene(Scene &);

  // tell the engine to finish up and then de-init
  void raise_stop_condition();

  // forcefully de-init the engine
  void stop();

private:
  static Engine *m_Singleton;

private:
  Engine(void);
  ~Engine(void);

  int _renderer_setup();

private:
  bool m_ShouldStop;

  Renderer *m_Renderer;
  SceneManager *m_SceneManager;
};

} // namespace fag

#endif // FAG_ENGINE_ENGINE_HPP
