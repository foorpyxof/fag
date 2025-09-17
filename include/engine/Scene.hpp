#ifndef FAG_ENGINE_SCENE_HPP
#define FAG_ENGINE_SCENE_HPP

#include <string>

namespace fag {

class SceneManager;

class Scene {
public:
  enum class LoadResult : int { Success = 0, Failure = -1 };
  typedef LoadResult (*LoadCallback)(Scene *);

public:
  void set_loading_callback(LoadCallback);
  void set_unloading_callback(LoadCallback);

  // use these methods in your Loading callback
  bool load_from_gltf(const char *path);

private:
  LoadCallback _load = nullptr;
  LoadCallback _unload = nullptr;

private:
  friend SceneManager;

  bool m_IsLoaded;
};

} // namespace fag

#endif // FAG_ENGINE_SCENE_HPP
