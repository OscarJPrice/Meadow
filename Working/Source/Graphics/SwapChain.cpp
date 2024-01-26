#include "Graphics/SwapChain.hpp"
#include "Graphics/PhysicalDeviceManager.hpp"
#include "ansi.h"
#include "Logging/Logging.hpp"


/**
 * @brief Constructs a SwapChain object.
 *
 * This constructor initializes a SwapChain object with the specified parameters.
 *
 * @param window The GLFW window handle.
 * @param physical_device The Vulkan physical device.
 * @param logical_device The Vulkan logical device.
 * @param surface The Vulkan surface.
 */
SwapChain::SwapChain(GLFWwindow* window, VkPhysicalDevice physical_device, 
VkDevice logical_device, VkSurfaceKHR surface) {

    this->logical_device = logical_device;

    // Query swap chain support details
    SwapChainSupportDetails swap_chain_support = PhysicalDeviceManager::querySwapChainSupport(physical_device, surface);

    // Choose the surface format and present mode
    VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);
    VkPresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.present_modes);

    // Determine the number of images in the swap chain
    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    // Create swap chain create info
    VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = chooseSwapExtent(window, swap_chain_support.capabilities),
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    };

    // Find queue families for graphics and presentation
    QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(physical_device, surface);

    uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

    if (!indices.isComplete()) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "SwapChain.cpp " ANSI_NORMAL "failed to find queue families!");
    }

    // Set sharing mode and queue family indices
    if (indices.graphics_family.value() != indices.present_family.value()) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    // Create the swap chain
    if (vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "SwapChain.cpp " ANSI_NORMAL "failed to create swap chain!");
    }   
}

/**
 * @brief Destroys a SwapChain object.
 *
 * This destructor destroys a SwapChain object.
 *
 */
SwapChain::~SwapChain() {
    vkDestroySwapchainKHR(logical_device, swapChain, nullptr);
}


/**
 * @brief Chooses the best surface format for the swap chain.
 *
 * This function iterates through the available surface formats and returns the first one that matches the desired format (VK_FORMAT_B8G8R8A8_SRGB) and color space (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR).
 * If no matching format is found, it returns the first available format in the list.
 *
 * @param available_formats The list of available surface formats.
 * @return The chosen surface format.
 */
VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
	// TODO(Add support for more formats)
    for (const auto& available_format : available_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB 
			&& available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return available_format;
		}
	}
	// for (const auto& available_format : available_formats) {
	// 	if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM 
	// 		&& available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
	// 		return available_format;
	// 	}
	// }
	return available_formats[0];
}

/**
 * @brief Chooses the best presentation mode for the swap chain.
 *
 * This function iterates through the available presentation modes and selects the best one.
 * The best mode is determined by choosing VK_PRESENT_MODE_IMMEDIATE_KHR if available,
 * otherwise VK_PRESENT_MODE_FIFO_KHR is used as the default mode.
 *
 * @param available_present_modes The list of available presentation modes.
 * @return The best presentation mode.
 */
VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) {
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& mode : available_present_modes) {
		if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			best_mode = mode;
            break;
		}
	}

	return best_mode;
}


/**
 * @brief Chooses the swap extent for the swap chain.
 *
 * This function determines the swap extent (i.e., the size of the swap chain images) based on the capabilities of the surface and the size of the window.
 * If the current extent is not set to a specific value, it uses the size of the window obtained from GLFW.
 * The chosen extent is then clamped to the minimum and maximum extents supported by the surface capabilities.
 *
 * @param window The GLFW window handle.
 * @param capabilities The surface capabilities obtained from the Vulkan API.
 * @return The chosen swap extent.
 */
VkExtent2D SwapChain::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {

    // Check if the current extent is already set
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        // Return the current extent if it is already set
        return capabilities.currentExtent;
    }
    else {
        // Get the width and height of the framebuffer
        int width, height;
        glfwGetFramebufferSize(window, &width, &height); //why the fuck are these ints? lord knows!

        // Create the actual extent based on the framebuffer size
        VkExtent2D actual_extents = {
            (uint32_t)width, 
            (uint32_t)height
        };

        // Clamp the actual extent to the minimum and maximum extents supported by the surface capabilities
        actual_extents.width = std::clamp(actual_extents.width, 
            capabilities.minImageExtent.width, 
            capabilities.maxImageExtent.width);
        
        actual_extents.height = std::clamp(actual_extents.height, 
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

        // Return the chosen swap extent
        return actual_extents;
    }
}

