// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/VulkanRenderer.hpp"

#include "dev/allocators.hpp"

#include "macros.hpp"

extern "C" {
#include "fpxlib3d/include/vk.h"
#include "fpxlib3d/include/vk/typedefs.h"
}

#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

#include <stdexcept>

static bool s_VulkanSettingsInitialized = false;

static Fpx3d_Vk_SwapchainRequirements s_SwapchainRequirements;
static VkSurfaceFormatKHR s_VulkanSurfaceFormats[1] = {
    {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};
static VkPresentModeKHR s_VulkanPresentModes[1] = {VK_PRESENT_MODE_FIFO_KHR};
static const char *s_VulkanGpuExtensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
static VkPhysicalDeviceFeatures s_DeviceFeatures{};

static Fpx3d_Vk_DescriptorSetBinding s_default3d_pipeline_bindings[1]{};
static Fpx3d_Vk_DescriptorSetBinding s_default3d_object_bindings[2]{};

static Fpx3d_Vk_DescriptorSetBinding s_default2d_pipeline_bindings[1]{};
static Fpx3d_Vk_DescriptorSetBinding s_default2d_object_bindings[2]{};

static int s_gpu_scoring_function(Fpx3d_Vk_Context *, Fpx3d_Vk_PhysicalDevice);
static struct fpx3d_wnd_dimensions s_window_resize_callback(void *window_ptr);

struct default3d_object_descriptor_bindings {
  glm::mat4 model;
};
struct default3d_pipeline_descriptor_bindings {
  glm::mat4 view;
  glm::mat4 projection;
};

struct default2d_object_descriptor_bindings {
  /* TODO: define this one */
};
struct default2d_pipeline_descriptor_bindings {
  /* TODO: define this one */
};

namespace fag {

/* static initializations */
VulkanRenderer *VulkanRenderer::m_Singleton = nullptr;

/* public static methods */
VulkanRenderer *VulkanRenderer::get_singleton(void) {
  if (nullptr == VulkanRenderer::m_Singleton)
    FAG_HEAP_CONSTRUCT(VulkanRenderer, VulkanRenderer::m_Singleton, ());

  return VulkanRenderer::m_Singleton;
}

void VulkanRenderer::destroy_singleton(void) {
  if (nullptr == VulkanRenderer::m_Singleton)
    return;

  delete VulkanRenderer::m_Singleton;
  VulkanRenderer::m_Singleton = nullptr;
}

#define INIT_WINDOW_WIDTH 500
#define INIT_WINDOW_HEIGHT 500

/* public methods implementations */

IMPLEMENT_THIS(void VulkanRenderer::render_frame(void), );

IMPLEMENT_THIS(std::weak_ptr<Renderer::Shader> VulkanRenderer::create_shader(
                   std::string &resource_path, ShaderStage stage_flags),
               UNUSED(resource_path);
               UNUSED(stage_flags); return {};);

void VulkanRenderer::select_render_context(size_t idx) {
  size_t real_index = _convert_render_context_index(idx);

  if (real_index >= m_Pipelines.size())
    return;

  m_SelectedPipeline = m_Pipelines[real_index];
}
IMPLEMENT_THIS(void VulkanRenderer::set_shapes(
                   const std::vector<Renderer::Mesh *> &shapes),
               UNUSED(shapes););

VulkanRenderer::VulkanRenderer(void)
    : m_Pipelines(Renderer::BASE_RENDER_CONTEXT_COUNT) {
  UNUSED(m_VulkanContext);

  if (!s_VulkanSettingsInitialized) {
    fpx3d_vk_set_required_presentmodes(&s_SwapchainRequirements,
                                       s_VulkanPresentModes,
                                       ARRAY_SIZE(s_VulkanPresentModes));
    fpx3d_vk_set_required_surfaceformats(&s_SwapchainRequirements,
                                         s_VulkanSurfaceFormats,
                                         ARRAY_SIZE(s_VulkanSurfaceFormats));

    s_DeviceFeatures.geometryShader = VK_TRUE;
    s_DeviceFeatures.samplerAnisotropy = VK_TRUE;

    s_default3d_pipeline_bindings[0].elementCount = 1;
    s_default3d_pipeline_bindings[0].elementSize =
        sizeof(struct default3d_pipeline_descriptor_bindings);
    s_default3d_pipeline_bindings[0].type = DESC_UNIFORM;
    s_default3d_pipeline_bindings[0].shaderStages = SHADER_STAGE_VERTEX;

    s_default3d_object_bindings[0].elementCount = 1;
    s_default3d_object_bindings[0].elementSize =
        sizeof(struct default3d_object_descriptor_bindings);
    s_default3d_object_bindings[0].type = DESC_UNIFORM;
    s_default3d_object_bindings[0].shaderStages = SHADER_STAGE_VERTEX;

    s_default3d_object_bindings[1].elementCount = 1;
    s_default3d_object_bindings[1].type = DESC_IMAGE_SAMPLER;
    s_default3d_object_bindings[1].shaderStages = SHADER_STAGE_FRAGMENT;

    // TODO: 2d

    // s_default2d_pipeline_bindings[0].elementCount = 1;
    // s_default2d_pipeline_bindings[0].elementSize =
    //     sizeof(struct default2d_pipeline_descriptor_bindings);
    // s_default2d_pipeline_bindings[0].type = DESC_UNIFORM;
    // s_default2d_pipeline_bindings[0].shaderStages = SHADER_STAGE_VERTEX;
    //
    // s_default2d_object_bindings[0].elementCount = 1;
    // s_default2d_object_bindings[0].elementSize =
    //     sizeof(struct default2d_object_descriptor_bindings);
    // s_default2d_object_bindings[0].type = DESC_UNIFORM;
    // s_default2d_object_bindings[0].shaderStages = SHADER_STAGE_VERTEX;
    //
    // s_default2d_object_bindings[1].elementCount = 1;
    // s_default2d_object_bindings[1].type = DESC_IMAGE_SAMPLER;
    // s_default2d_object_bindings[1].shaderStages = SHADER_STAGE_FRAGMENT;

    s_VulkanSettingsInitialized = true;
  }

  glfwInit();

  _vulkan_setup();
  _glfw_setup();
  _gpu_setup();
  _create_base_pipelines();
}
VulkanRenderer::~VulkanRenderer(void) {}

void VulkanRenderer::_vulkan_setup(void) {
  {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "F.A.G. Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);

    m_VulkanContext.appInfo = app_info;

    m_VulkanContext.instanceLayers = nullptr;
    m_VulkanContext.instanceLayerCount = 0;
    m_VulkanContext.lgpuExtensions = s_VulkanGpuExtensions;
    m_VulkanContext.lgpuExtensionCount = ARRAY_SIZE(s_VulkanGpuExtensions);

    uint32_t glfw_extension_count = 0;
    m_VulkanContext.instanceExtensions =
        glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    m_VulkanContext.instanceExtensionCount = glfw_extension_count;

    fpx3d_vk_set_custom_pointer(&m_VulkanContext, this);
  }

  m_WindowContext.sizeCallback = s_window_resize_callback;
  fpx3d_vk_init_context(&m_VulkanContext, &m_WindowContext);

  // double buffering
  m_VulkanContext.constants.maxFramesInFlight = 2;

  fpx3d_vk_create_instance(&m_VulkanContext);
}

void VulkanRenderer::_glfw_setup(void) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  GLFWwindow *win = nullptr;

  win = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT,
                         "F.A.G. Game Window", nullptr, nullptr);

  if (nullptr == win) {
    throw std::runtime_error("Could not create game window");
  }

  VkResult surface_result = glfwCreateWindowSurface(
      m_VulkanContext.vkInstance, win, nullptr, &m_VulkanContext.vkSurface);

  if (VK_SUCCESS > surface_result) {
    throw std::runtime_error("Could not create Vulkan instance");
  }

  fpx3d_wnd_set_window_pointer(&m_WindowContext, win);
}

void VulkanRenderer::_gpu_setup(void) {
  fpx3d_vk_select_gpu(&m_VulkanContext, s_gpu_scoring_function);

  fpx3d_vk_allocate_logicalgpus(&m_VulkanContext, 1);
  fpx3d_vk_create_logicalgpu_at(&m_VulkanContext, 0, s_DeviceFeatures, 1, 1, 1);

  Fpx3d_Vk_LogicalGpu *lgpu = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  if (nullptr == lgpu)
    throw std::runtime_error("Failed to initialize Vulkan GPU");
}

void VulkanRenderer::_create_default3d_pipeline_layout(void) {
  Fpx3d_Vk_LogicalGpu *lgpu = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  Fpx3d_Vk_DescriptorSetLayout default3d_descriptor_set_layouts[2]{};

  default3d_descriptor_set_layouts[0] = fpx3d_vk_create_descriptor_set_layout(
      s_default3d_pipeline_bindings, ARRAY_SIZE(s_default3d_pipeline_bindings),
      lgpu);
  default3d_descriptor_set_layouts[1] = fpx3d_vk_create_descriptor_set_layout(
      s_default3d_object_bindings, ARRAY_SIZE(s_default3d_object_bindings),
      lgpu);

  m_Default3DPipelineLayout = fpx3d_vk_create_pipeline_layout(
      default3d_descriptor_set_layouts,
      ARRAY_SIZE(default3d_descriptor_set_layouts), lgpu);
}

void VulkanRenderer::_create_default2d_pipeline_layout(void) {
  Fpx3d_Vk_LogicalGpu *lgpu = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  Fpx3d_Vk_DescriptorSetLayout default2d_descriptor_set_layouts[2]{};

  default2d_descriptor_set_layouts[0] = fpx3d_vk_create_descriptor_set_layout(
      s_default2d_pipeline_bindings, ARRAY_SIZE(s_default2d_pipeline_bindings),
      lgpu);

  default2d_descriptor_set_layouts[0] = fpx3d_vk_create_descriptor_set_layout(
      s_default2d_object_bindings, ARRAY_SIZE(s_default2d_object_bindings),
      lgpu);

  m_Default2DPipelineLayout = fpx3d_vk_create_pipeline_layout(
      default2d_descriptor_set_layouts,
      ARRAY_SIZE(default2d_descriptor_set_layouts), lgpu);
}

void VulkanRenderer::_create_pipeline_layouts(void) {
  _create_default3d_pipeline_layout();

  // TODO: implement
  // _create_default2d_pipeline_layout();
}

void VulkanRenderer::_create_base_pipelines(void) {
  Fpx3d_Vk_LogicalGpu *lgpu = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  fpx3d_vk_allocate_commandpools(lgpu, 2);
  fpx3d_vk_create_commandpool_at(lgpu, 0, TRANSFER_POOL);
  fpx3d_vk_create_commandpool_at(lgpu, 1, GRAPHICS_POOL);
  fpx3d_vk_allocate_commandbuffers_at_pool(lgpu, 0, 4);
  fpx3d_vk_allocate_commandbuffers_at_pool(lgpu, 1, 4);

  fpx3d_vk_create_swapchain(&m_VulkanContext, lgpu, s_SwapchainRequirements);
  Fpx3d_Vk_Swapchain *sc = fpx3d_vk_get_current_swapchain(lgpu);

  fpx3d_vk_allocate_renderpasses(lgpu, 1);
  fpx3d_vk_create_renderpass_at(lgpu, 0, true, &m_VulkanContext);

  Fpx3d_Vk_RenderPass *renderpass = fpx3d_vk_get_renderpass_at(lgpu, 0);

  if (nullptr == renderpass)
    throw std::runtime_error("Failed to create Vulkan Renderpass");

  fpx3d_vk_create_framebuffers(sc, &m_VulkanContext, lgpu, renderpass);
}

Renderer::Mesh *VulkanRenderer::Mesh::clone(void) {
  VulkanRenderer::Mesh *cloned_mesh;
  FAG_HEAP_CONSTRUCT(VulkanRenderer::Mesh, cloned_mesh, ());

  cloned_mesh->m_VulkanShapeBuffer = m_VulkanShapeBuffer;

  return cloned_mesh;
}

} // namespace fag

// STATIC FUNCTIONS DEFINED BELOW

static int s_gpu_scoring_function(Fpx3d_Vk_Context *vk_ctx,
                                  Fpx3d_Vk_PhysicalDevice gpu) {
  int score = 200;
  float multiplier = 1.0f;

  switch (gpu.properties.deviceType) {
  case VK_PHYSICAL_DEVICE_TYPE_CPU:
    multiplier = 0.2f;
    break;

  case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    multiplier = 2.0f;
    break;

  case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    multiplier = 1.3f;
    break;

  default:
    multiplier = 1.0f;
    break;
  }

  if (!gpu.features.geometryShader)
    return 0;

  {
    Fpx3d_Vk_SwapchainProperties sc_props =
        fpx3d_vk_create_swapchain_properties(vk_ctx, gpu.handle,
                                             s_SwapchainRequirements);

    if (!(sc_props.presentModeValid && sc_props.surfaceFormatValid))
      return 0;
  }

  return score * multiplier;
}

static struct fpx3d_wnd_dimensions s_window_resize_callback(void *window_ptr) {
  struct fpx3d_wnd_dimensions retval{};
  if (nullptr == window_ptr)
    return retval;

  int w, h;
  w = h = 0;

  glfwGetWindowSize(static_cast<GLFWwindow *>(window_ptr), &w, &h);

  retval.width = w;
  retval.height = h;

  return retval;
}
