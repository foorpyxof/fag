// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "error/IError.hpp"
#include "fag.hpp"

#include "macros.hpp"
#include "os/File.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

int engine_test(void) {
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

  return EXIT_SUCCESS;
}

int file_test(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "pass filename to read" << std::endl;
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  char *charbuf = nullptr;

  try {
    fag::OS::FileBuffer filebuf(filename, fag::OS::FileAccessMode::ReadWrite);

    const char newtext[] = "abcdefghijklmnopqrstuvwxyz";
    filebuf.advance(10);
    filebuf.write((void *)newtext, sizeof(newtext) - 1);
    filebuf.delete_bytes(2);
    filebuf.rewind(6);
    filebuf.delete_bytes(10);
    filebuf.advance(30);
    filebuf.delete_bytes(100);

    // filebuf.save_to_disk("./temp.txt");

    charbuf = new char[filebuf.get_size() + 1]{};

    filebuf.set_buffer_index(FAG_FILE_POSITION_START);
    filebuf.read(charbuf, FAG_FILE_POSITION_END);

    std::cout << charbuf << std::endl;

  } catch (const fag::Error::IError &err) {
    if (charbuf)
      delete[] charbuf;

    std::cerr << err.what() << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  // return engine_test();
  return file_test(argc, argv);
}
