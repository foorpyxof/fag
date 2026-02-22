// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Allocator.hpp"
#include "core/Entity3D.hpp"
#include "core/Globals.hpp"
#include "core/Renderer.hpp"
#include "core/Shader.hpp"
#include "core/Vulkan/GlfwWindow.hpp"
#include "core/Vulkan/Mesh.hpp"
#include "core/Vulkan/MeshInstance.hpp"
#include "core/Vulkan/Renderer.hpp"
#include "core/Vulkan/Shader.hpp"
#include "core/Vulkan/Window.hpp"
#include "error/Fatal.hpp"
#include "error/IError.hpp"
#include "fag.hpp"

#include "macros.hpp"
#include "os/File.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

static fag::Vulkan::Renderer *g_vulkan_renderer;
static std::shared_ptr<fag::Window> g_main_window;

static void *s_custom_allocator(size_t);
static void s_custom_deleter(void *);

static fag::Allocator custom_alloc{s_custom_allocator, s_custom_deleter};

fag::Scene::LoadResult my_scene_loader(fag::Scene &to_load) {
  UNUSED(to_load);

  return fag::Scene::LoadResult::Success;
}

fag::Scene::LoadResult my_scene_unloader(fag::Scene &to_load) {
  UNUSED(to_load);

  return fag::Scene::LoadResult::Success;
}

int vulkan_renderer_setup(void) {
  g_main_window = std::make_shared<fag::Vulkan::GlfwWindow>();
  g_vulkan_renderer = new fag::Vulkan::Renderer(
      std::dynamic_pointer_cast<fag::Vulkan::Window>(g_main_window));
  // g_vulkan_renderer->use_window(g_main_window);

  g_Engine->assign_renderer(g_vulkan_renderer);

  std::cout << "Renderer is VulkanRenderer? ";
  std::cout << (g_Renderer->is<fag::Vulkan::Renderer>() ? "yes" : "no")
            << std::endl;

  // 3d render context
  {
    fag::OS::FileBuffer vertex_shader_file("./shaders/default.vert.spv",
                                           fag::OS::FileAccessMode::Read);
    fag::OS::FileBuffer fragment_shader_file("./shaders/default.frag.spv",
                                             fag::OS::FileAccessMode::Read);

    std::shared_ptr<fag::Shader> vertex_shader =
        g_Renderer->create_shader(vertex_shader_file, fag::ShaderStage::Vertex);
    std::shared_ptr<fag::Shader> fragment_shader = g_Renderer->create_shader(
        fragment_shader_file, fag::ShaderStage::Fragment);

    std::vector<fag::Shader *> shaders{};
    shaders.push_back(vertex_shader.get());
    shaders.push_back(fragment_shader.get());

    fag::RendercontextCreationInfo info{};
    info.type = fag::RendercontextCreationInfo::Type::Rendercontext3D;
    info.shaderArray = shaders.data();
    info.shaderCount = 2;
    info.contextDescriptors = std::vector<fag::RendercontextDescriptor>();
    info.objectDescriptors = std::vector<fag::RendercontextDescriptor>();

    g_Renderer->create_render_context(info);
  }

  // select the 3D render context
  g_Renderer->select_render_context(0);

  return EXIT_SUCCESS;
}

int entities_setup(void) {
  fag::MeshCreationInfo mesh_info{};
  std::shared_ptr<fag::Mesh> meshptr = g_Renderer->create_mesh(mesh_info);

  std::shared_ptr<fag::Entity3D> test_entity{new fag::Entity3D};
  test_entity->set_mesh(meshptr);

  std::weak_ptr<fag::Entity> my_entities[1];

  std::vector<std::weak_ptr<fag::Entity>> entities;
  entities.assign(my_entities, my_entities + 1);
  g_Renderer->set_entities(entities);

  return EXIT_SUCCESS;
}

int engine_test(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  g_Engine->set_custom_allocator(custom_alloc);

  bool setup_success = EXIT_SUCCESS == vulkan_renderer_setup() &&
                       EXIT_SUCCESS == entities_setup();

  if (!setup_success)
    return EXIT_FAILURE;

  fag::Scene my_scene;
  my_scene.set_loader(my_scene_loader);
  my_scene.set_unloader(my_scene_unloader);

  g_Engine->add_scene(my_scene);

  g_Engine->start();

  fag::Engine::destroy_singleton();

  delete g_vulkan_renderer;

  return EXIT_SUCCESS;
}

int window_test(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  g_main_window = std::make_shared<fag::Vulkan::GlfwWindow>(400, 400);

  g_main_window->set_window_title("F.A.G. Window test");
  std::cout << "Window title:\t\t" << g_main_window->get_window_title()
            << std::endl;
  std::cout << "Width:\t\t\t" << g_main_window->get_dimensions().width
            << "\nHeight:\t\t\t" << g_main_window->get_dimensions().height
            << std::endl;
  std::cout << "Has window closed:\t" << g_main_window->has_closed()
            << std::endl;
  g_main_window->poll_events();

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
  try {
    return engine_test(argc, argv);
    // return window_test(argc, argv);
    // return file_test(argc, argv);
  } catch (const fag::Error::IError &err) {
    std::cout << err.what() << std::endl;
  } catch (const fag::Error::Fatal &err) {
    std::cout << err.what() << std::endl;
  }
}

void *s_custom_allocator(size_t bytes) { return ::malloc(bytes); }
void s_custom_deleter(void *ptr) { ::free(ptr); }
