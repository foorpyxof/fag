// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Vulkan/Renderer.hpp"
#include "fag.hpp"

#include "macros.hpp"

#include <cstdio>
#include <cstdlib>
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
  MyEntity3D() { printf("hello !\n"); }
  ~MyEntity3D() { printf("goodbye !\n"); }
};

int main(void) {
  g_Engine->assign_renderer(new fag::Vulkan::Renderer);

  std::cout << "Renderer is VulkanRenderer? ";
  std::cout << (g_Renderer->is<fag::Vulkan::Renderer>() ? "yes" : "no")
            << std::endl;

  fag::Entity3D *ent_3d = new fag::Entity3D;

  std::shared_ptr<fag::Entity3D> my_ent_3d =
      fag::Entity::promote<fag::Entity3D, MyEntity3D>(ent_3d);

  delete ent_3d;

  std::cout << my_ent_3d.get() << std::endl;

  fag::Scene my_scene;
  my_scene.set_loader(my_scene_loader);
  my_scene.set_unloader(my_scene_unloader);

  g_Engine->add_scene(my_scene);

  g_Engine->start();

  fag::Engine::destroy_singleton();
}
