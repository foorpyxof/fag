#include "engine.hpp"
#include "engine/Scene.hpp"

extern "C" {
#include "macros.h"
}

#include <cstdio>

using namespace fag;

Scene::LoadResult my_scene_loader(Scene *to_load) {
  UNUSED(to_load);

  return Scene::LoadResult::Success;
}

Scene::LoadResult my_scene_unloader(Scene *to_load) {
  UNUSED(to_load);

  return Scene::LoadResult::Success;
}

int main(void) {
  Engine *engine = Engine::get_singleton();
  printf("%p\n", (void *)engine);

  engine->start();

  Scene my_scene;
  my_scene.set_loading_callback(my_scene_loader);
  my_scene.set_unloading_callback(my_scene_unloader);

  engine->add_scene(my_scene);

  Engine::destroy_singleton();
}
