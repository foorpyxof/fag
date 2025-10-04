// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "engine/VulkanRenderer.hpp"

#include "macros.hpp"
#include <stdexcept>

extern "C" {
#include "fpxlib3d/include/vk.h"
}

#include "glfw/include/GLFW/glfw3.h"

static bool s_VulkanSettingsInitialized = false;

static Fpx3d_Vk_SwapchainRequirements s_SwapchainRequirements;
static VkSurfaceFormatKHR s_VulkanSurfaceFormats[1] = {
    {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};
static VkPresentModeKHR s_VulkanPresentModes[1] = {VK_PRESENT_MODE_FIFO_KHR};
static const char *s_VulkanGpuExtensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
static VkPhysicalDeviceFeatures s_DeviceFeatures{};

static int s_gpu_scoring_function(Fpx3d_Vk_Context *, VkPhysicalDevice);

namespace fag {

// XXX: change this when there's more default pipelines!
// (based on enum class VulkanRenderer::PipelineIndex)
#define BASE_PIPELINE_COUNT 2

#define INIT_WINDOW_WIDTH 500
#define INIT_WINDOW_HEIGHT 500

VulkanRenderer::VulkanRenderer(void) : m_Pipelines(BASE_PIPELINE_COUNT) {
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

    s_VulkanSettingsInitialized = true;
  }

  glfwInit();

  _vulkan_setup();
  _glfw_setup();
  _gpu_setup();
}
VulkanRenderer::~VulkanRenderer(void) {}

IMPLEMENT_THIS(void VulkanRenderer::render_frame(void), );

Renderer::Backend VulkanRenderer::get_backend(void) const {
  return Renderer::Backend::Vulkan;
}

void VulkanRenderer::select_pipeline(size_t idx) {
  size_t real_index;

  switch (idx) {
  case PipelineIndex::Default2D:
  case PipelineIndex::Default3D:
    real_index = std::numeric_limits<size_t>::max() - idx;
    break;

  default:
    real_index = idx + BASE_PIPELINE_COUNT;
    break;
  }

  if (real_index >= m_Pipelines.size())
    return;

  m_SelectedPipeline = m_Pipelines[real_index];
}
IMPLEMENT_THIS(void VulkanRenderer::set_shapes(
                   const std::vector<VulkanRenderer::Shape *> &shapes),
               UNUSED(shapes););

void VulkanRenderer::_vulkan_setup() {
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

  fpx3d_vk_init_context(&m_VulkanContext, &m_WindowContext);

  // double buffering
  m_VulkanContext.constants.maxFramesInFlight = 2;

  fpx3d_vk_create_instance(&m_VulkanContext);
}

void VulkanRenderer::_glfw_setup() {
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

void VulkanRenderer::_gpu_setup() {
  fpx3d_vk_select_gpu(&m_VulkanContext, s_gpu_scoring_function);

  fpx3d_vk_allocate_logicalgpus(&m_VulkanContext, 1);
  fpx3d_vk_create_logicalgpu_at(&m_VulkanContext, 0, s_DeviceFeatures, 1, 1, 1);

  Fpx3d_Vk_LogicalGpu *lgpu = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  if (nullptr == lgpu)
    throw std::runtime_error("Failed to initialize Vulkan GPU");
}

} // namespace fag

// STATIC FUNCTIONS DEFINED BELOW

static int s_gpu_scoring_function(Fpx3d_Vk_Context *vk_ctx,
                                  VkPhysicalDevice gpu) {
  int score = 200;
  float multiplier = 1.0f;

  VkPhysicalDeviceProperties dev_props{};
  VkPhysicalDeviceFeatures dev_feats{};

  vkGetPhysicalDeviceProperties(gpu, &dev_props);
  vkGetPhysicalDeviceFeatures(gpu, &dev_feats);

  switch (dev_props.deviceType) {
  case VK_PHYSICAL_DEVICE_TYPE_CPU:
    multiplier = 0.2f;
    break;

  case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    multiplier = 2.0f;

  case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    multiplier = 1.3f;

  default:
    multiplier = 1.0f;
    break;
  }

  if (!dev_feats.geometryShader)
    return 0;

  {
    Fpx3d_Vk_SwapchainProperties sc_props =
        fpx3d_vk_create_swapchain_properties(vk_ctx, gpu,
                                             s_SwapchainRequirements);

    if (!(sc_props.presentModeValid && sc_props.surfaceFormatValid))
      return 0;
  }

  return score * multiplier;
}
