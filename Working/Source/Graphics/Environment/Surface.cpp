#include <stdexcept>
#include "Surface.hpp"
#include "ansi.h"



/**
 * @brief Construct a new Surface:: Surface object
 * 
 * @param instance Vulkan Instance
 * @param window GLFW Window
 */
Surface::Surface(const VkInstance& instance, GLFWwindow* window) : instance(instance) {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface)) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT 
            "Surface.cpp " ANSI_NORMAL "Failed to create window surface!");
    }
}

/**
 * @brief Destroy the Surface:: Surface object
 * 
 */
Surface::~Surface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}