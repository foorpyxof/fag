#include "core/Vulkan/Window.hpp"

#define VK_NO_PROTOTYPES
#include "fpxlib3d/modules/Vulkan-Headers/include/vulkan/vulkan_core.h"

namespace fag {
namespace Vulkan {

Window::~Window(void) {}

VkSurfaceKHR Window::get_vulkan_surface(void) const { return m_VulkanSurface; }

void Window::_set_vulkan_instance(VkInstance &vk) { m_VulkanInstance = vk; }
void Window::_set_vulkan_surface(VkSurfaceKHR &srfc) { m_VulkanSurface = srfc; }

} // namespace Vulkan
} // namespace fag
