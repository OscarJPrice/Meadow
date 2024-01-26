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
    VkSwapchainKHR swap_chain;
    const VkDevice& logical_device;
    std::vector<VkImage> swap_chain_images;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;


public:

    SwapChain(GLFWwindow* window, const VkPhysicalDevice& physical_device, 
        const VkDevice& logical_device, const VkSurfaceKHR& surface);

    ~SwapChain();

    inline operator VkSwapchainKHR&() { return swap_chain; }

    inline operator VkFormat&() { return swap_chain_image_format; }

    inline operator VkExtent2D&() { return swap_chain_extent; }

    inline operator std::vector<VkImage>&() { return swap_chain_images; }

    inline std::vector<VkImage>& images() { return swap_chain_images; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif // MEADOW_SWAPCHAIN_HPP
