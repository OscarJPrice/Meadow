#ifndef MEADOW_SURFACE_HPP
#define MEADOW_SURFACE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/**
 * @brief Wrapper class for the surface
 * 
 */
class Surface {
    VkSurfaceKHR surface;
    VkInstance& instance;

public:

    Surface(VkInstance& instance, GLFWwindow* window);

    ~Surface();

    inline operator VkSurfaceKHR() {
        return surface;
    }

    inline operator VkSurfaceKHR&() {
        return surface;
    }


};
#endif // SURFACE_HPP
