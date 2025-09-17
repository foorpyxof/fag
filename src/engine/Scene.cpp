#include "engine/Scene.hpp"

namespace fag {

void Scene::set_loading_callback(Scene::LoadCallback cb) { _load = cb; }
void Scene::set_unloading_callback(Scene::LoadCallback cb) { _unload = cb; }

} // namespace fag
