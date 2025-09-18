// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "engine.hpp"
#include "engine/Entity3D.hpp"
#include "engine/Renderer.hpp"
#include "engine/Scene.hpp"

extern "C" {
#include "macros.h"
}

#include <cstdio>

fag::Scene::LoadResult my_scene_loader(fag::Scene *to_load) {
  UNUSED(to_load);

  return fag::Scene::LoadResult::Success;
}

fag::Scene::LoadResult my_scene_unloader(fag::Scene *to_load) {
  UNUSED(to_load);

  return fag::Scene::LoadResult::Success;
}

class MyEntity3D : public fag::Entity3D {
public:
  MyEntity3D() { printf("fagballs\n"); }
  ~MyEntity3D() { printf("goodbye !\n"); }
};

int main(void) {
  fag::Engine *engine = fag::Engine::get_singleton();
  printf("%p\n", (void *)engine);

  fag::Scene my_scene;
  my_scene.set_loader(my_scene_loader);
  my_scene.set_unloader(my_scene_unloader);

  engine->add_scene(my_scene);

  // it uses Vulkan by default (since that is the only option)
  // but you know, just in case
  engine->renderBackend = fag::Renderer::Backend::Vulkan;
  engine->start();

  fag::Engine::destroy_singleton();
}
