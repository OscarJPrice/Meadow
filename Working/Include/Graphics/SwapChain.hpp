#ifndef MEADOW_SWAPCHAIN_HPP
#define MEADOW_SWAPCHAIN_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueUtils.hpp"


/**
 * @brief Struct containing information about the swap chain
 * 
 */
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

/**
 * @brief Class for managing the swap chain
 * 
 */
class SwapChain {
    VkSwapchainKHR swapChain;
    VkDevice logical_device;


public:

    SwapChain(GLFWwindow* window, VkPhysicalDevice physical_device, 
        VkDevice logical_device, VkSurfaceKHR surface);

    ~SwapChain();

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif // MEADOW_SWAPCHAIN_HPP
