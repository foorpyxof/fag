// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "engine/SceneManager.hpp"
#include "engine/Scene.hpp"

extern "C" {
#include "debug.h"
}

#include <stdexcept>
#include <string>

#define SCENE_INDEX_CHECK(_idx_var)                                            \
  if (_idx_var >= m_Scenes.size()) {                                           \
    return false;                                                              \
  }

namespace fag {

bool SceneManager::load_scene_at(size_t index) {
  SCENE_INDEX_CHECK(index);

  Scene &scene = m_Scenes[index];

  Scene::LoadCallback load_func = scene._load;
  if (nullptr == load_func) {
    FAG_WARN("Scene at index %" LONG_FORMAT "u has no loading function", index);
    return false;
  }

  if (true == scene.m_IsLoaded) {
    // scene is already loaded, so we unload first
    unload_scene_at(index);
  }

  FAG_DEBUG("Loading scene at index %" LONG_FORMAT "u", index);
  if (Scene::LoadResult::Success != scene._load(&scene)) {
    std::string msg = "Failed to load scene at index ";
    msg += std::to_string(index);

    throw std::runtime_error(msg);
  }

  scene.m_IsLoaded = true;

  return true;
}

bool SceneManager::unload_scene_at(size_t index) {
  SCENE_INDEX_CHECK(index);

  Scene &scene = m_Scenes[index];

  if (false == scene.m_IsLoaded)
    // scene is not loaded
    return true;

  if (nullptr == scene._unload) {
    FAG_WARN("Scene at index %" LONG_FORMAT "u has no unloading function",
             index);
    return false;
  }

  FAG_DEBUG("Unloading scene at index %" LONG_FORMAT "u", index);

  scene._unload(&scene);
  scene.m_IsLoaded = false;

  return true;
}

bool SceneManager::activate_scene_at(size_t index) {
  SCENE_INDEX_CHECK(index);

  Scene &scene = m_Scenes[index];

  if (false == scene.m_IsLoaded) {
    // scene is not loaded- oops

    FAG_ERROR("Unable to activate scene at index %" LONG_FORMAT
              "u. Scene not loaded.",
              index);

    return false;
  }

  m_ActiveIndex = index;

  return true;
}

size_t SceneManager::get_scene_count() const { return m_Scenes.size(); }
size_t SceneManager::get_active_scene_index() const { return m_ActiveIndex; }
const std::vector<Scene> &SceneManager::get_scenes() const { return m_Scenes; }

size_t SceneManager::append_scene(Scene &to_append) {
  m_Scenes.push_back(to_append);
  return m_Scenes.size() - 1;
}

} // namespace fag
