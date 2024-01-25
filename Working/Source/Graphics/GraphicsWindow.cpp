#include "Graphics/GraphicsWindow.hpp"
#include <Logging/Logging.hpp>
#include <cstring>
#include "ansi.h"
#include <set>
#include <algorithm>
#include <limits>

//////////////////////////////////////////////////////////////////////////////
// The following code are helper functions, invisible outside of this file. //
//////////////////////////////////////////////////////////////////////////////
/**
 * @brief Creates a debug utils messenger extension.
 *
 * This function creates a debug utils messenger extension using the provided parameters.
 *
 * @param instance The Vulkan instance.
 * @param create_info Pointer to a structure containing the debug utils messenger create info.
 * @param allocator Pointer to a structure containing the allocation callbacks.
 * @param debug_messenger Pointer to a debug utils messenger handle.
 * @return VkResult The result of the function call.
 */
VkResult createDebugUtilsMessengerExtension(
	VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* debug_messenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) 
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func) {
        return func(instance, create_info, allocator, debug_messenger);
		return VK_SUCCESS;
    } 
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
/**
 * @brief Destroys the debug utils messenger extension.
 *
 * @param instance The Vulkan instance.
 * @param allocator The allocation callbacks.
 * @param debug_messenger The debug utils messenger extension.
 * @return VkResult The result of the operation.
 */
VkResult destroyDebugUtilsMessengerExtension(
	VkInstance instance, 
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT debug_messenger
) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func) {
        func(instance, debug_messenger, allocator);
		return VK_SUCCESS;
	} 
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


/////////////////////////////////////////////////////////
// The following code is for the GraphicsWindow class. //
/////////////////////////////////////////////////////////
/**
 * @brief Constructs a GraphicsWindow object with the specified width, height, and name.
 *
 * @param width The width of the graphics window.
 * @param height The height of the graphics window.
 * @param name The name of the graphics window.
 */
GraphicsWindow::GraphicsWindow(uint32_t width, uint32_t height, const char* name) {
    Log::init();
	initializeWindow(width, height, name);
    initializeGraphicsENV(name);
    run();
}

/**
 * @brief Destructor for the GraphicsWindow class.
 * 
 * This destructor is responsible for cleaning up the resources used by the GraphicsWindow object.
 * It destroys the Vulkan instance, the GLFW window, and terminates the GLFW library.
 * If validation layers are enabled, it also destroys the debug messenger extension.
 */
GraphicsWindow::~GraphicsWindow() {
	if (ENABLE_VALIDATION_LAYERS) {
		destroyDebugUtilsMessengerExtension(instance, nullptr, debug_messenger);
	}
	vkDestroyDevice(device, nullptr);

	vkDestroySurfaceKHR(instance, surface, nullptr);

    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}


/**
 * Runs the graphics window event loop.
 * This function continuously polls for events until the window is closed.
 */
void GraphicsWindow::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

/**
 * @brief Creates a Vulkan instance for the graphics window.
 * 
 * This function initializes the Vulkan instance by setting up the application information,
 * instance creation info, and enabling necessary extensions and validation layers.
 * It also checks for extension support and throws an error if any required extensions are missing.
 * Finally, it calls vkCreateInstance to create the Vulkan instance.
 * 
 * @throws std::runtime_error if the instance creation fails.
 */
void GraphicsWindow::createVulkanInstance(const char* name) {

    VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = name,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.pApplicationInfo = &app_info,
        .enabledLayerCount = 0
    };
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;

	if (ENABLE_VALIDATION_LAYERS) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();

		debugMessengerPopulateCreateInfo(debug_create_info);
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
	}

	std::vector<const char*> extensions = getInstanceRequiredExtensions();
	if (!checkExtensionsSupport(extensions)) {
		throw std::runtime_error("not all extensions found!");
	}

	#ifdef __APPLE__
		//very fucking necessary
		create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; 
	#endif

	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	create_info.ppEnabledExtensionNames = extensions.data();

    auto result = vkCreateInstance(&create_info, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

/**
 * @brief Initializes the graphics window.
 * 
 * @param width The width of the window.
 * @param height The height of the window.
 */
void GraphicsWindow::initializeWindow(uint32_t width, uint32_t height, const char* name) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

/**
 * @brief Initializes the graphics environment.
 * 
 * This function creates a Vulkan instance, sets up the debug callback system, and picks the physical device.
 */
void GraphicsWindow::initializeGraphicsENV(const char* name) {
    createVulkanInstance(name);
	setupDebugCallbackSys();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
}

/**
 * Calculates the suitability score of a given physical device for graphics rendering.
 * The score is based on the device's properties and features.
 *
 * @param device The physical device to evaluate.
 * @return The suitability score of the device.
 */
uint32_t GraphicsWindow::rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures device_features;
	
	vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);

	//Create a score based on the device's properties.
	uint32_t score = 0;

	//Discrete GPUs have a significant performance advantage
	if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	//Maximum possible size of textures affects graphics quality
	score += device_properties.limits.maxImageDimension2D;

	return score;
}

/**
 * Checks if a given Vulkan physical device is suitable for rendering graphics on the graphics window.
 * 
 * @param device The Vulkan physical device to check.
 * @return True if the device is suitable, false otherwise.
 */
bool GraphicsWindow::isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = GraphicsWindow::findQueueFamilies(device);

	bool extensions_supported = checkDeviceExtensionSupport(device);

	bool swap_chain_adequate = false;
	if (extensions_supported) {
		SwapChainSupportDetails swap_chain_support = querySwapChainSupport(device);
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	}

	return indices.isComplete() && swap_chain_adequate;// && extensions_supported;
}

/**
 * @brief Checks if a given Vulkan physical device supports the required device extensions.
 * 
 * @param device The Vulkan physical device to check.
 * @return True if the device supports the required extensions, false otherwise.
 */
bool GraphicsWindow::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	std::set<const char*> required_extensions(device_extensions.begin(), device_extensions.end());

	for (const char* extension : device_extensions) {
		for (const auto& available_extension : available_extensions) {
			if (strcmp(extension, available_extension.extensionName) == 0) {
				goto next;
			}
		}
		#ifndef NDEBUG
			Log::error << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "Extension " << extension << " not supported!" << std::endl;
			throw std::runtime_error("required extension not supported!");
		#endif
		return false;
		next: continue;

	}

	return true;

} 

/**
 * @brief Structure to store queue family indices for a graphics window.
 */
GraphicsWindow::QueueFamilyIndices GraphicsWindow::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices{};
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());
		int i = 0; 
		for (const auto& queue_family : queue_families) {
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

			if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphics_family = i;
			}

			if (present_support) {
				indices.present_family = i;
			}

			i++;
		}
	}
	return indices;
}


/**
 * Queries the swap chain support details for a given physical device.
 *
 * @param device The Vulkan physical device to query.
 * @return The swap chain support details.
 */
GraphicsWindow::SwapChainSupportDetails GraphicsWindow::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

	if (present_mode_count != 0) {
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
	}

    return details;
}



VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
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
 * @brief Chooses a swap chain present mode.
 * 
 * This function chooses a swap chain present mode from the available present modes.
 * It first checks if VK_PRESENT_MODE_MAILBOX_KHR is available, and if so, returns it.
 * If not, it checks if VK_PRESENT_MODE_IMMEDIATE_KHR is available, and if so, returns it.
 * If neither are available, it returns VK_PRESENT_MODE_FIFO_KHR.
 * 
 * @param available_present_modes A vector of available present modes.
 * @return VkPresentModeKHR The chosen present mode.
 */
VkPresentModeKHR GraphicsWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) {
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& mode : available_present_modes) {
		if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			best_mode = mode;
		}
	}

	return best_mode;
}

/**
 * @brief Chooses a swap chain extent.
 * 
 * This function chooses a swap chain extent from the available capabilities.
 * If the current extent is not set to the maximum value, it returns the current extent.
 * Otherwise, it returns the extent that best matches the window width and height.
 * 
 * @param capabilities The swap chain capabilities.
 * @return VkExtent2D The chosen swap chain extent.
 */
VkExtent2D GraphicsWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
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

void GraphicsWindow::createSwapChain() {
	SwapChainSupportDetails swap_chain_support = querySwapChainSupport(physical_device);

	VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);

	VkExtent2D extent = chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

	if (swap_chain_support.capabilities.maxImageCount != 0 && 
	image_count > swap_chain_support.capabilities.maxImageCount) {
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = image_count,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	};

}

void GraphicsWindow::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

/**
 * @brief Picks the best physical device with Vulkan support.
 *
 * This function enumerates all the physical devices available and rates their suitability based on certain criteria.
 * The device with the highest score is selected as the best device.
 * If no suitable device is found, an exception is thrown.
 */
void GraphicsWindow::pickPhysicalDevice() {
	uint32_t count;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);

	if (count == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());
	
	uint32_t highest_score = 0;
	VkPhysicalDevice best_device = VK_NULL_HANDLE;

	for (const auto& device : devices) {
		uint32_t score = rateDeviceSuitability(device);
		if (score > highest_score) {
			if (isDeviceSuitable(device)) {
				best_device = device;
				highest_score = score;
			}
			else if (!isDeviceSuitable(best_device)) {
				best_device = device;
				highest_score = score;
			}
		}
	}

	if (best_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	physical_device = best_device;
}

/**
 * @brief Sets up the debug callback system for the graphics window.
 * 
 * This function initializes the debug callback system if validation layers are enabled.
 * It populates the create info structure for the debug messenger and creates the debug messenger.
 * If the creation of the debug messenger fails, it throws a runtime error.
 */
void GraphicsWindow::setupDebugCallbackSys() {
	if (!ENABLE_VALIDATION_LAYERS) return;

	VkDebugUtilsMessengerCreateInfoEXT create_info;
	debugMessengerPopulateCreateInfo(create_info);

	if (createDebugUtilsMessengerExtension(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}

}

/**
 * @brief Populates the VkDebugUtilsMessengerCreateInfoEXT structure for setting up the debug messenger.
 * 
 * @param create_info The VkDebugUtilsMessengerCreateInfoEXT structure to be populated.
 */
void GraphicsWindow::debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
	create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debugCallback;
	create_info.pUserData = nullptr;
}

/**
 * Creates the logical device for the graphics window.
 */
void GraphicsWindow::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physical_device);

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = {
		indices.graphics_family.value(),
		indices.present_family.value()
	};

	float queue_priority = 1.0f;

	for (const auto& queue_family : unique_queue_families) {
		VkDeviceQueueCreateInfo queue_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = queue_family,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
		};
		queue_create_infos.push_back(queue_create_info);
	}

	VkPhysicalDeviceFeatures device_features{};
	VkDeviceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = queue_create_infos.data(),
		.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
		.pEnabledFeatures = &device_features,
		.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
		.ppEnabledExtensionNames = device_extensions.data(),
	};

	// Compatibility with older versions of Vulkan
	if (ENABLE_VALIDATION_LAYERS) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	}
	else {
		create_info.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &present_queue);
}

/**
 * Checks if the given extensions are supported by the graphics window.
 * 
 * @param extensions A vector of extension names to check for support.
 * @return True if all extensions are supported, false otherwise.
 */
bool GraphicsWindow::checkExtensionsSupport(std::vector<const char*> extensions) {
    std::vector<VkExtensionProperties> available_extensions;
    {
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        available_extensions.resize(extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
    }

    //this is used in place of std::includes because it allows for debug info.
    for (const char* extension : extensions) {
        for (const auto& available_extension : available_extensions) {
            if (strcmp(extension, available_extension.extensionName) == 0) {
                goto next;
            }
        }
        return false;
        next: continue;
    }
    return true;
}


/**
 * @brief Returns a vector of const char* containing the required extensions for the graphics window.
 * 
 * This function retrieves the required instance extensions using glfwGetRequiredInstanceExtensions() and adds them to a vector.
 * If the platform is Apple, it also adds the VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME extension.
 * If ENABLE_VALIDATION_LAYERS is true, it also adds the VK_EXT_DEBUG_UTILS_EXTENSION_NAME extension.
 * 
 * @return std::vector<const char*> A vector of const char* containing the required extensions.
 */
std::vector<const char*> GraphicsWindow::getInstanceRequiredExtensions() {
    uint32_t glfw_extension_count;
    const char** glfw_extensions =
        glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    #ifdef __APPLE__ 
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif

    if (ENABLE_VALIDATION_LAYERS) {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/**
 * @brief Checks if the required validation layers are supported by the Vulkan instance.
 * 
 * @return true if all validation layers are supported, false otherwise.
 */
bool GraphicsWindow::checkValidationLayerSupport()
{

    std::vector<VkLayerProperties> available_layers;
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        available_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 
    }

    //this is used in place of std::includes because it allows for debug info.
    for (const char* layer : validation_layers) {
        for (VkLayerProperties available_layer : available_layers) {
            if (strcmp(layer, available_layer.layerName) == 0) {
                goto next;
            }
        }
        return false;
        next: continue;
    }
    return true;
}



/**
 * @brief Debug callback function for the GraphicsWindow class.
 *
 * This function is called when a Vulkan debug message is generated. 
 * It provides information about the severity, type, and content of the message.
 *
 * @param message_severity The severity level of the debug message.
 * @param message_type The type of the debug message.
 * @param p_callback_data A pointer to a structure containing the debug message data.
 * @param p_user_data A pointer to user-defined data.
 * @return VkBool32 Returns a boolean value indicating whether the debug message should be handled by the application.
 */
VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsWindow::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
	VkDebugUtilsMessageTypeFlagsEXT message_type, 
	const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
	void *p_user_data
) {
	VkBool32 result = VK_FALSE;
	std::ofstream* stream; // I would use a reference, but I can't!
	switch (message_severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			stream = &Log::verbose;
			*stream << FAINT "[VERBOSE] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			stream = &Log::info;
			*stream << GREEN_FG "[INFO] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cout<< YELLOW_FG "[WARNING] " ANSI_NORMAL << "Written to Logs/warning.log" << std::endl;
			stream = &Log::warning;
			*stream << YELLOW_FG "[WARNING] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cout << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL << "Written to Logs/error.log" << std::endl;
			stream = &Log::error;
			*stream << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL;
			result = VK_TRUE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			std::cout << MAGENTA_FG_BRIGHT  "[UNKNOWN] " ANSI_NORMAL;
			break;
	}

	switch (message_type) {
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			*stream << "[GENERAL]: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			*stream << "[VALIDATION]: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			*stream << "[PERFORMANCE]: ";
			break;
	}

	*stream << p_callback_data->pMessage << std::endl;
	for (uint32_t i = 0; i < p_callback_data->objectCount; i++) {
		if (p_callback_data->pObjects[i].pObjectName) {
			*stream << p_callback_data->pObjects[i].pObjectName << std::endl;
		}
	}

	return result;
    
}