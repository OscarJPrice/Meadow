#ifndef MEADOW_SWAPCHAIN_HPP
#define MEADOW_SWAPCHAIN_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueUtils.hpp"
#include "GraphicsContext.hpp"

/**
 * @brief Class for managing the swap chain
 * 
 */
class Swapchain {
    VkSwapchainKHR swapchain;
    const VkDevice& logical_device;
    SwapchainSupportDetails swapchain_support;
    VkSurfaceFormatKHR surface_format;
    VkFormat image_format;
    VkExtent2D extent;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    std::vector<VkFramebuffer> framebuffers;
    const GraphicsContext& graphics_context;
    VkRenderPass* render_pass;


public:

    Swapchain(const GraphicsContext& graphics_context);

    ~Swapchain();

    inline void setRenderPass(VkRenderPass& render_pass) { this->render_pass = &render_pass; }

    inline const VkFormat& getFormat() { return image_format; }

private:

    void createSwapChain();

    void createFramebuffers(const GraphicsContext& graphics_context, const VkRenderPass& render_pass);

    void createImageViews();

    void recreate();

    void cleanup();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
    

};

#endif // MEADOW_SWAPCHAIN_HPP
