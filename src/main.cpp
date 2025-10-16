// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Renderer.hpp"
#include "core/VulkanRenderer.hpp"
#include "fag.hpp"

extern "C" {
#include "macros.hpp"
}

#include <cstdio>
#include <iostream>

fag::Scene::LoadResult my_scene_loader(fag::Scene &to_load) {
  UNUSED(to_load);

  return fag::Scene::LoadResult::Success;
}

fag::Scene::LoadResult my_scene_unloader(fag::Scene &to_load) {
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
  fag::Renderer *renderer = fag::VulkanRenderer::get_singleton();
  engine->assign_renderer(renderer);

  std::cout << "Renderer is VulkanRenderer? ";
  std::cout << (renderer->is<fag::VulkanRenderer>() ? "yes" : "no")
            << std::endl;

  fag::Scene my_scene;
  my_scene.set_loader(my_scene_loader);
  my_scene.set_unloader(my_scene_unloader);

  engine->add_scene(my_scene);

  engine->start();

  fag::Engine::destroy_singleton();
}
