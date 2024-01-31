#include <algorithm>
#include "Swapchain.hpp"
#include "ansi.h"
#include "Logging.hpp"
#include "RenderPass.hpp"


/**
 * @brief Constructs a Swapchain object.
 *
 * This constructor initializes a Swapchain object with the specified parameters.
 *
 * @param window The GLFW window handle.
 * @param physical_device The Vulkan physical device.
 * @param logical_device The Vulkan logical device.
 * @param surface The Vulkan surface.
 * 
 * A swapchain is essentially an image holder. These held images are to be "swapped"
 * into the screen buffer. Prior to the swap, the images are rendered to. There are
 * two big ways a swapchain can work.
 * * Double buffering, where the first image is the one currently displayes to the 
 * screen buffer (see Window.cpp for more info on the window).
 * * Triple buffering, where there are three images, and the first image is the one
 * currently displayed to the screen buffer, the second is the one currently being 
 * rendered, and the third is the one after that. It is the logical continuation of
 * double buffering.
 * 
 */
Swapchain::Swapchain(const GraphicsContext& graphics_context) :
    logical_device(graphics_context.getLogicalDevice()),
    swapchain_support(graphics_context.queryPhysicalSwapChainSupport()),     // Query swap chain support details
    surface_format(chooseSwapSurfaceFormat(swapchain_support.formats)),                       // Choose the surface format
    image_format(surface_format.format),             // Choose the image format
    extent(chooseSwapExtent(graphics_context.getWindow(), swapchain_support.capabilities)),   // Choose the swap extent)
    graphics_context(graphics_context),
    render_pass(nullptr)
{
    createSwapChain();

    createImageViews();

    //createFramebuffers(graphics_context, render_pass);
}

/**
 * @brief Destroys a Swapchain object.
 *
 * This destructor destroys a Swapchain object.
 *
 */
Swapchain::~Swapchain() {
    cleanup();
}

void Swapchain::createSwapChain() {
    VkPresentModeKHR present_mode = chooseSwapPresentMode(swapchain_support.present_modes);

    // Determine the number of images in the swap chain
    uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;

    if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount) {
        image_count = swapchain_support.capabilities.maxImageCount;
    }

    extent = chooseSwapExtent(graphics_context.getWindow(), swapchain_support.capabilities);

    // Create swap chain create info
    VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = graphics_context.getSurface(),
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    };

    // Find queue families for graphics and presentation
    QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(graphics_context.getPhysicalDevice(), graphics_context.getSurface());

    uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

    if (!indices.isComplete()) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "Swapchain.cpp " ANSI_NORMAL "failed to find queue families!");
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

    create_info.preTransform = swapchain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    // Create the swap chain
    if (vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swapchain)) {
        throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "Swapchain.cpp " ANSI_NORMAL "failed to create swap chain!");
    }
    
}

void Swapchain::createFramebuffers(const GraphicsContext& graphics_context, const VkRenderPass& render_pass)
{
    framebuffers.resize(image_views.size());

    for (uint32_t i = 0; i < image_views.size(); i++) {


        VkFramebufferCreateInfo framebuffer_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = &image_views[i],
            .width = extent.width,
            .height = extent.height,
            .layers = 1
        };
        if (vkCreateFramebuffer(graphics_context.getLogicalDevice(), &framebuffer_info, nullptr, &framebuffers[i])) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

void Swapchain::createImageViews() {
    image_views.resize(images.size());
    int i = 0;
    for (const auto& image : images) {
        VkImageViewCreateInfo image_view_create_info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = image_format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        if (vkCreateImageView(graphics_context.getLogicalDevice(), &image_view_create_info, nullptr,
            image_views.data() + i)) {
            throw std::runtime_error("Failed to create image views!");
        }
        i++;
    }

}

void Swapchain::cleanup() {
    for (auto& framebuffer : framebuffers) {
        vkDestroyFramebuffer(logical_device, framebuffer, nullptr);
    }

    for (auto& image_view : image_views) {
        vkDestroyImageView(graphics_context.getLogicalDevice(), image_view, nullptr);
    }
    
    for (auto& image : images) {
        vkDestroyImage(logical_device, image, nullptr);
    } 

    vkDestroySwapchainKHR(logical_device, swapchain, nullptr);
}

void Swapchain::recreate() {
    vkDeviceWaitIdle(graphics_context.getLogicalDevice());

    cleanup();

    createSwapChain();

    createImageViews();

    if (render_pass != nullptr) {
        createFramebuffers(graphics_context, *render_pass);
    }
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
VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
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
VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) {
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
VkExtent2D Swapchain::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {

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

