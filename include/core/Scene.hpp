// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_SCENE_HPP
#define FAG_CORE_SCENE_HPP

#include "./BaseObject.hpp"

#include <string>

namespace fag {

class Scene : public BaseObject {
public:
  enum class LoadResult : int;
  typedef LoadResult (*LoadCallback)(Scene &);

public:
  void set_loader(LoadCallback);
  void set_unloader(LoadCallback);

  // use these methods in your Loading callback
  bool load_from_gltf(const char *path);

  bool is_valid(void) const;

private:
  LoadCallback _load = nullptr;
  LoadCallback _unload = nullptr;

private:
  friend class SceneManager;

  bool m_IsLoaded;

public:
  enum class LoadResult : int { Success = 0, Failure = -1 };
};

} // namespace fag

#endif // FAG_CORE_SCENE_HPP
