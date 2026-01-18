// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Renderer.hpp"
#include "core/Entity.hpp"
// #include "core/Entity2D.hpp"
#include "core/Entity3D.hpp"
#include "core/Mesh.hpp"
#include "core/MeshInstance.hpp"
#include "core/Transforms.hpp"
#include "core/Vertex.hpp"
#include "core/Vulkan/Mesh.hpp"
#include "core/Vulkan/MeshInstance.hpp"
#include "core/Vulkan/Shader.hpp"
#include "debug.h"
#include "dev/allocators.hpp"
#include "dev/smartptrs.hpp"
#include "error/Generic.hpp"
#include "error/Internal.hpp"

#include "macros.hpp"

#include "core/Vulkan/Renderer.hpp"

extern "C" {
#include "fpxlib3d/include/vk/descriptors.h"
#include "fpxlib3d/include/vk/pipeline.h"
#include "fpxlib3d/include/vk/queues.h"
#include "fpxlib3d/include/vk/renderpass.h"
#include "fpxlib3d/include/vk/shaders.h"
#include "fpxlib3d/include/vk/shape.h"
#include "fpxlib3d/include/vk/typedefs.h"
#include "fpxlib3d/include/vk/utility.h"
#include "fpxlib3d/include/vk/vertex.h"
}

#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

static bool s_VulkanSettingsInitialized = false;

static Fpx3d_Vk_SwapchainRequirements s_SwapchainRequirements;
static VkSurfaceFormatKHR s_VulkanSurfaceFormats[1] = {
    {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};
static VkPresentModeKHR s_VulkanPresentModes[1] = {VK_PRESENT_MODE_FIFO_KHR};
static const char *s_VulkanGpuExtensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
static VkPhysicalDeviceFeatures s_DeviceFeatures{};

static int s_gpu_scoring_function(Fpx3d_Vk_Context *, Fpx3d_Vk_PhysicalDevice);
static struct fpx3d_wnd_dimensions s_window_resize_callback(void *window_ptr);

static void
s_create_binding_list(std::vector<Fpx3d_Vk_DescriptorSetBinding> &,
                      const std::vector<fag::RendercontextDescriptor> &,
                      fag::RendercontextCreationInfo::Type);

namespace fag {
namespace Vulkan {

#define INIT_WINDOW_WIDTH 500
#define INIT_WINDOW_HEIGHT 500

Renderer::Renderer(void) : m_VulkanContext({}), m_LogicalGpuPtr(nullptr) {
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
  _prepare_framebuffers();
}
Renderer::~Renderer(void) {
  glfwDestroyWindow(static_cast<GLFWwindow *>(m_WindowContext.pointer));
  glfwTerminate();
  FAG_TODO("properly destroy Vulkan::Renderer in the destructor");
}

bool Renderer::window_has_closed(void) {
  return (m_WindowContext.pointer == nullptr) ||
         glfwWindowShouldClose(
             static_cast<GLFWwindow *>(m_WindowContext.pointer));
}

void Renderer::render_frame(void) {
  glfwPollEvents();

  if (nullptr == m_LogicalGpuPtr) {
    FAG_ERROR(fag::Vulkan::Renderer, "m_LogicalGpuPtr is NULL");
    throw fag::Error::Internal("the Vulkan renderer encountered an error while "
                               "preparing to render a frame",
                               __FILE__, __LINE__);
  }

  if (0 == m_LogicalGpuPtr->pipelineCapacity)
    return;

  VkQueue *graphics_queue =
      fpx3d_vk_get_queue_at(m_LogicalGpuPtr, 0, GRAPHICS_QUEUE);
  VkQueue *present_queue =
      fpx3d_vk_get_queue_at(m_LogicalGpuPtr, 0, PRESENT_QUEUE);

  fpx3d_vk_draw_frame(
      &m_VulkanContext, m_LogicalGpuPtr, m_LogicalGpuPtr->pipelines,
      m_LogicalGpuPtr->pipelineCapacity, graphics_queue, present_queue);
}

void Renderer::select_render_context(size_t idx) {
  size_t real_index = _convert_render_context_index(idx);

  if (!m_LogicalGpuPtr) {
    // this should never be the case
    throw Error::Internal("the Vulkan renderer's logical GPU device was "
                          "uninitialized, for some reason",
                          __FILE__, __LINE__);
  }

  if (real_index >= m_LogicalGpuPtr->pipelineCapacity)
    throw Error::Generic("Vulkan pipeline index out of range");

  m_SelectedPipelineIndex = real_index;
}

size_t Renderer::create_render_context(
    const fag::RendercontextCreationInfo &creation_info) {
  std::vector<Fpx3d_Vk_DescriptorSetBinding> pipeline_bindings(
      creation_info.contextDescriptors.size()
      /* and then +1 for view/projection matrices */
      + 1);
  std::vector<Fpx3d_Vk_DescriptorSetBinding> object_bindings(
      creation_info.objectDescriptors.size()
      /* and then +1 for the model matrix */
      + 1);

  // First we create descriptor set layouts

  /*
   * Set up descriptor set bindings
   */

  // first are the global, pipeline-wide bindings
  s_create_binding_list(pipeline_bindings, creation_info.contextDescriptors,
                        creation_info.type);

  // now come the object-unique bindings
  s_create_binding_list(object_bindings, creation_info.objectDescriptors,
                        creation_info.type);

  /*
   * Then create the layouts
   */
  Fpx3d_Vk_DescriptorSetLayout ds_layouts[2]{};

  ds_layouts[0] /* pipeline_descriptors */ =
      fpx3d_vk_create_descriptor_set_layout(
          pipeline_bindings.data(), pipeline_bindings.size(), m_LogicalGpuPtr);

  ds_layouts[1] /* object_descriptors */ =
      fpx3d_vk_create_descriptor_set_layout(
          object_bindings.data(), object_bindings.size(), m_LogicalGpuPtr);

  Fpx3d_Vk_PipelineLayout pl =
      fpx3d_vk_create_pipeline_layout(ds_layouts, 2, m_LogicalGpuPtr);

  Fpx3d_Vk_SpirvFile *spirvs =
      new Fpx3d_Vk_SpirvFile[creation_info.shaderCount]{};
  for (size_t i = 0; i < creation_info.shaderCount; ++i) {
    spirvs[i] =
        dynamic_cast<fag::Vulkan::Shader *>(creation_info.shaderArray[i])
            ->m_SpirvData;
  }

  Fpx3d_Vk_ShaderModuleSet shaders{};
  fpx3d_vk_load_shadermodules(spirvs, creation_info.shaderCount,
                              m_LogicalGpuPtr, &shaders);

  static Fpx3d_Vk_VertexAttribute vertex_attributes_3d[]{
      {_fpx3d_vk_vertex_attr::VEC3_32BIT_SFLOAT,
       offsetof(struct fag::Vertex3D, position)},
      {_fpx3d_vk_vertex_attr::VEC3_32BIT_SFLOAT,
       offsetof(struct fag::Vertex3D, color)},
      {_fpx3d_vk_vertex_attr::VEC2_32BIT_SFLOAT,
       offsetof(struct fag::Vertex3D, textureCoordinate)}};

  static Fpx3d_Vk_VertexBinding vertex_binding_3d{
      vertex_attributes_3d, ARRAY_SIZE(vertex_attributes_3d),
      sizeof(struct fag::Vertex3D)};

  static Fpx3d_Vk_VertexAttribute vertex_attributes_2d[]{
      {_fpx3d_vk_vertex_attr::VEC2_32BIT_SFLOAT,
       offsetof(struct fag::Vertex2D, position)},
      {_fpx3d_vk_vertex_attr::VEC3_32BIT_SFLOAT,
       offsetof(struct fag::Vertex2D, color)},
      {_fpx3d_vk_vertex_attr::VEC2_32BIT_SFLOAT,
       offsetof(struct fag::Vertex2D, textureCoordinate)}};

  static Fpx3d_Vk_VertexBinding vertex_binding_2d{
      vertex_attributes_2d, ARRAY_SIZE(vertex_attributes_2d),
      sizeof(struct fag::Vertex2D)};

  fpx3d_vk_allocate_pipelines(m_LogicalGpuPtr,
                              m_LogicalGpuPtr->pipelineCapacity + 1);

  if (fag::RendercontextCreationInfo::Type::Rendercontext3D ==
      creation_info.type) {
    fpx3d_vk_create_graphics_pipeline_at(
        m_LogicalGpuPtr, m_LogicalGpuPtr->pipelineCapacity - 1, &pl,
        fpx3d_vk_get_renderpass_at(m_LogicalGpuPtr, 0), &shaders,
        &vertex_binding_3d, 1);
  } else if (fag::RendercontextCreationInfo::Type::Rendercontext2D ==
             creation_info.type) {
    fpx3d_vk_create_graphics_pipeline_at(
        m_LogicalGpuPtr, m_LogicalGpuPtr->pipelineCapacity - 1, &pl,
        fpx3d_vk_get_renderpass_at(m_LogicalGpuPtr, 0), &shaders,
        &vertex_binding_2d, 1);
  } else {
    // what
    FAG_ERROR(fag::Vulkan::Renderer,
              "invalid value for Vulkan Pipeline type (2D or 3D)")
    fpx3d_vk_destroy_pipeline_layout(&pl, m_LogicalGpuPtr);
    fpx3d_vk_destroy_descriptor_set_layout(&ds_layouts[0], m_LogicalGpuPtr);
    fpx3d_vk_destroy_descriptor_set_layout(&ds_layouts[1], m_LogicalGpuPtr);
    fpx3d_vk_destroy_shadermodules(&shaders, m_LogicalGpuPtr);
    throw fag::Error::Generic(
        "invalid value passed for Vulkan Rendercontext type");
  }

  Fpx3d_Vk_Pipeline *p = fpx3d_vk_get_pipeline_at(
      m_LogicalGpuPtr, m_LogicalGpuPtr->pipelineCapacity - 1);

  fpx3d_vk_create_pipeline_descriptors(p, pipeline_bindings.data(),
                                       pipeline_bindings.size(),
                                       &m_VulkanContext, m_LogicalGpuPtr);

  struct _pipeline_properties new_pipeline_props{};
  new_pipeline_props.pipelineType = creation_info.type;

  m_PipelineProperties.resize(m_LogicalGpuPtr->pipelineCapacity);
  m_PipelineProperties[m_LogicalGpuPtr->pipelineCapacity - 1] =
      new_pipeline_props;
  // m_PipelineProperties.push_back(new_pipeline_props);

  return m_LogicalGpuPtr->pipelineCapacity - 1;
}

void Renderer::set_entities(
    const std::vector<std::weak_ptr<fag::Entity>> &entities) {
  std::vector<const Fpx3d_Vk_Shape *> shapes_to_set(entities.size());

  for (const std::weak_ptr<fag::Entity> &e : entities) {
    std::shared_ptr<fag::Entity> s_ptr = e.lock();
    if (!s_ptr)
      continue;

    if (m_PipelineProperties[m_SelectedPipelineIndex].pipelineType ==
            fag::RendercontextCreationInfo::Type::Rendercontext3D &&
        s_ptr->is<fag::Entity3D>()) {
      // we found a 3d entity while we are in a 3d environment. let's add it to
      // the renderer
      const fag::Entity3D *e3d =
          dynamic_cast<const fag::Entity3D *>(s_ptr.get());
      if (nullptr == e3d)
        throw fag::Error::Internal("what the fuck", __FILE__, __LINE__);

      std::shared_ptr<const fag::MeshInstance> mi_ptr =
          e3d->get_meshinstance().lock();
      const fag::Vulkan::MeshInstance *vk_mi_ptr =
          dynamic_cast<const fag::Vulkan::MeshInstance *>(mi_ptr.get());

      shapes_to_set.push_back(&vk_mi_ptr->m_VulkanShape);
    }
    // else if (m_PipelineProperties[m_SelectedPipelineIndex].pipelineType ==
    //                fag::RendercontextCreationInfo::Type::Rendercontext2D &&
    //            e->is<fag::Entity2D>()) {
    // }
    else {
      // ermmmmmm
      FAG_TODO("implement Vulkan::Renderer::set_entities()");
    }
  }
}

IMPLEMENT_THIS(std::shared_ptr<fag::Mesh> Renderer::create_mesh(),
               return nullptr;);

std::shared_ptr<fag::MeshInstance>
Renderer::create_meshinstance(fag::MeshInstanceCreationInfo &create_info) {
  fag::Vulkan::MeshInstance *mi_to_return = nullptr;

  if (std::shared_ptr<const fag::Mesh> mesh_ptr =
          create_info.meshPointer.lock()) {
    const fag::Vulkan::Mesh *raw_ptr =
        dynamic_cast<const fag::Vulkan::Mesh *>(mesh_ptr.get());

    if (nullptr == raw_ptr)
      throw fag::Error::Generic("Vulkan renderer was given a non-Vulkan mesh",
                                __FILE__, __LINE__);

    FAG_HEAP_CONSTRUCT(fag::Vulkan::MeshInstance, mi_to_return, (mesh_ptr));

    if (!mi_to_return->m_VulkanShape.isValid)
      throw fag::Error::Generic(
          "Vulkan renderer failed to create drawable object");
  } else
    throw fag::Error::Internal("Vulkan renderer could not claim pointer to "
                               "Mesh for creation of MeshInstance",
                               __FILE__, __LINE__);

  return fag::_dev::create_shared_ptr(mi_to_return);
}

void Renderer::destroy_mesh(fag::Mesh &mesh_ref) {
  UNUSED(mesh_ref);
  FAG_TODO("implement fag::Vulkan::Renderer::destroy_mesh()");
  mesh_ref.invalidate();
}

void Renderer::destroy_meshinstance(fag::MeshInstance &meshinstance_ref) {
  UNUSED(meshinstance_ref);
  FAG_TODO("implement fag::Vulkan::Renderer::destroy_meshinstance()");
  meshinstance_ref.invalidate();
}

void Renderer::_vulkan_setup(void) {
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

void Renderer::_glfw_setup(void) {
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

void Renderer::_gpu_setup(void) {
  fpx3d_vk_select_gpu(&m_VulkanContext, s_gpu_scoring_function);

  fpx3d_vk_allocate_logicalgpus(&m_VulkanContext, 1);
  fpx3d_vk_create_logicalgpu_at(&m_VulkanContext, 0, s_DeviceFeatures, 1, 1, 1);

  m_LogicalGpuPtr = fpx3d_vk_get_logicalgpu_at(&m_VulkanContext, 0);

  if (nullptr == m_LogicalGpuPtr)
    throw std::runtime_error("Failed to initialize Vulkan GPU");
}

void Renderer::_prepare_framebuffers(void) {
  fpx3d_vk_allocate_commandpools(m_LogicalGpuPtr, 2);
  fpx3d_vk_create_commandpool_at(m_LogicalGpuPtr, 0, TRANSFER_POOL);
  fpx3d_vk_create_commandpool_at(m_LogicalGpuPtr, 1, GRAPHICS_POOL);
  fpx3d_vk_allocate_commandbuffers_at_pool(m_LogicalGpuPtr, 0, 4);
  fpx3d_vk_allocate_commandbuffers_at_pool(m_LogicalGpuPtr, 1, 4);

  fpx3d_vk_create_swapchain(&m_VulkanContext, m_LogicalGpuPtr,
                            s_SwapchainRequirements);
  Fpx3d_Vk_Swapchain *sc = fpx3d_vk_get_current_swapchain(m_LogicalGpuPtr);

  fpx3d_vk_allocate_renderpasses(m_LogicalGpuPtr, 1);
  fpx3d_vk_create_renderpass_at(m_LogicalGpuPtr, 0, true, &m_VulkanContext);

  Fpx3d_Vk_RenderPass *renderpass =
      fpx3d_vk_get_renderpass_at(m_LogicalGpuPtr, 0);

  if (nullptr == renderpass)
    throw std::runtime_error("Failed to create Vulkan Renderpass");

  fpx3d_vk_create_framebuffers(sc, &m_VulkanContext, m_LogicalGpuPtr,
                               renderpass);
}

void Renderer::_teardown(void) {
  // destroy everything; Vulkan resources, Vulkan logical GPU(s), etc.

  /*
   * TODO: destroy resources owned by logical GPU
   */

  fpx3d_vk_destroy_logicalgpu_at(&m_VulkanContext, 0);
}

} // namespace Vulkan
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

static void
s_create_binding_list(std::vector<Fpx3d_Vk_DescriptorSetBinding> &output,
                      const std::vector<fag::RendercontextDescriptor> &from,
                      fag::RendercontextCreationInfo::Type pipeline_type) {
  int binding_index = 0;

  output[binding_index].type = DESC_UNIFORM;
  output[binding_index].elementCount = 1;
  output[binding_index].shaderStages = SHADER_STAGE_VERTEX;
  if (pipeline_type == fag::RendercontextCreationInfo::Type::Rendercontext2D) {
    // output[binding_index].elementSize =
  } else if (pipeline_type ==
             fag::RendercontextCreationInfo::Type::Rendercontext3D) {
    output[binding_index].elementSize =
        sizeof(struct fag::ViewProjectionMatrices3D);
  } else
    --binding_index;

  ++binding_index;

  for (size_t i = 0; i < from.size(); ++i, ++binding_index) {
    if (from[i].type == fag::RendercontextDescriptor::Type::Uniform)
      output[binding_index].type = DESC_UNIFORM;
    else if (from[i].type == fag::RendercontextDescriptor::Type::Sampler2D) {
      output[binding_index].type = DESC_IMAGE_SAMPLER;
      // TODO: add texture data to this binding
    }

    output[binding_index].elementSize = from[i].size;
    output[binding_index].elementCount = from[i].count;
  }
}
