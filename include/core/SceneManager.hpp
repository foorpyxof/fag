// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_SCENEMANAGER_HPP
#define FAG_CORE_SCENEMANAGER_HPP

#include "./Scene.hpp"

#include <cstddef>
#include <vector>

namespace fag {

class SceneManager {
public:
  bool load_scene_at(size_t index);
  bool unload_scene_at(size_t index);
  bool activate_scene_at(size_t index);

  size_t get_scene_count() const;
  size_t get_active_scene_index() const;
  const std::vector<Scene> &get_scenes() const;

  // also returns the index of the new scene
  size_t append_scene(Scene &);

private:
  size_t m_ActiveIndex;
  std::vector<Scene> m_Scenes;
};

} // namespace fag

#endif // FAG_CORE_SCENEMANAGER_HPP
