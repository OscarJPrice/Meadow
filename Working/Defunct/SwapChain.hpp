#ifndef MEADOW_SWAPCHAIN_HPP
#define MEADOW_SWAPCHAIN_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueUtils.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities {};
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
    SwapChainSupportDetails swap_chain_support;
    VkSurfaceFormatKHR surface_format;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    std::vector<VkImage> swap_chain_images;

public:

    SwapChain(GLFWwindow* window, const VkPhysicalDevice& physical_device, 
        const VkDevice& logical_device, const VkSurfaceKHR& surface);

    ~SwapChain();

    inline operator VkSwapchainKHR&() { return swap_chain; }

    inline operator VkSwapchainKHR*() { return &swap_chain; }

    inline const VkFormat& format() const { return swap_chain_image_format; }

    inline const std::vector<VkImage>& images() const { return swap_chain_images; }

    inline const VkExtent2D& extent() const { return swap_chain_extent; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif // MEADOW_SWAPCHAIN_HPP
