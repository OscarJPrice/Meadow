#include "Graphics/SwapChain.hpp"
#include "Graphics/PhysicalDeviceManager.hpp"
#include "ansi.h"
#include "Logging/Logging.hpp"

SwapChain::SwapChain(GLFWwindow* window, VkPhysicalDevice physical_device, 
    VkDevice logical_device, VkSurfaceKHR surface) {

    this->logical_device = logical_device;

    SwapChainSupportDetails swap_chain_support = PhysicalDeviceManager::querySwapChainSupport(physical_device, surface);

    VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);
    VkPresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.present_modes);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

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

    QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(physical_device, surface);

    uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

    if (!indices.isComplete()) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "SwapChain.cpp " ANSI_NORMAL "failed to find queue families!");
    }

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

    if (vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "SwapChain.cpp " ANSI_NORMAL "failed to create swap chain!");
    }   
}

SwapChain::~SwapChain() {
    vkDestroySwapchainKHR(logical_device, swapChain, nullptr);
}

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

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) {
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& mode : available_present_modes) {
		if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			best_mode = mode;
		}
	}

	return best_mode;
}


VkExtent2D SwapChain::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height); //why the fuck are these ints? lord knows!

		VkExtent2D actual_extents = {
			(uint32_t)width, 
			(uint32_t)height
		};

		actual_extents.width = std::clamp(actual_extents.width, 
			capabilities.minImageExtent.width, 
			capabilities.maxImageExtent.width);
		
		actual_extents.height = std::clamp(actual_extents.height, 
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);

		return actual_extents;
	}
}