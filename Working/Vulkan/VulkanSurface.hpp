#pragma once

#define GLFW_INCLUDE_VULKAN
#include "VulkanInstance.hpp"
#include <GLFW/glfw3.h>

class VulkanSurface {
public:
  GLFWwindow *window;
  VkSurfaceKHR vk_surface;
  const VulkanInstance &instance;

  VulkanSurface(const VulkanInstance &instance);

  inline operator bool() {
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
  }

  ~VulkanSurface();
};
