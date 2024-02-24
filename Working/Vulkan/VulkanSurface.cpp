#include "VulkanSurface.hpp"
#include "Debug-Macros.hpp"

VulkanSurface::VulkanSurface(const VulkanInstance &instance)
    : instance(instance) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(800, 600, CONSTANTS::APP_NAME, nullptr, nullptr);

  ATTEMPT_VK(glfwCreateWindowSurface(instance.vk_instance, window, nullptr,
                                     &vk_surface));
}

VulkanSurface::~VulkanSurface() {
  vkDestroySurfaceKHR(instance.vk_instance, vk_surface, nullptr);
  glfwDestroyWindow(window);
}
