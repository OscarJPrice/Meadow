#include <stdexcept>
#include "Graphics/Surface.hpp"
#include "ansi.h"

Surface::Surface(VkInstance instance, GLFWwindow* window) {
    this->instance = instance;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "Surface.cpp " ANSI_NORMAL "Failed to create window surface!");
    }
}

Surface::~Surface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}