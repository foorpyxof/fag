#include "engine/Scene.hpp"

namespace fag {

void Scene::set_loader(Scene::LoadCallback cb) { _load = cb; }
void Scene::set_unloader(Scene::LoadCallback cb) { _unload = cb; }

} // namespace fag
