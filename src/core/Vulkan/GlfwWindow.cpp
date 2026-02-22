// clang-format off
#define VK_NO_PROTOTYPES
#include "fpxlib3d/modules/Vulkan-Headers/include/vulkan/vulkan_core.h"
// clang-format on

#include "core/Vulkan/GlfwWindow.hpp"
#include "error/Fatal.hpp"

namespace fag {
namespace Vulkan {

GlfwWindow::GlfwWindow(void) : fag::GlfwWindow() {}
GlfwWindow::GlfwWindow(int _width, int _height)
    : fag::GlfwWindow(_width, _height) {}

GlfwWindow::~GlfwWindow(void) {}

void GlfwWindow::create_vulkan_surface(VkInstance vulkan) {
  VkSurfaceKHR new_surface = VK_NULL_HANDLE;
  if (VK_SUCCESS != glfwCreateWindowSurface(vulkan, this->_get_glfw_window(),
                                            nullptr, &new_surface))
    throw fag::Error::Fatal("could not create Vulkan surface for game window",
                            __FILE__, __LINE__);

  this->_set_vulkan_instance(vulkan);
  this->_set_vulkan_surface(new_surface);
}

} // namespace Vulkan
} // namespace fag
