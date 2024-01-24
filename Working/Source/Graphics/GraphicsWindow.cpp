#include "Graphics/GraphicsWindow.hpp"
#include <cstring>
#include "ansi.h"

//////////////////////////////////////////////////////
// The following code is for helper functions.
constexpr VkResult createDebugUtilsMessengerExtension(
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

constexpr VkResult destroyDebugUtilsMessengerExtension(
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


//////////////////////////////////////////////////////
// The following code is for the GraphicsWindow class.

GraphicsWindow::GraphicsWindow(int width, int height, const char* name) : name(name) {
    initializeWindow(width, height);
    initializeGraphicsENV();
    run();
}

GraphicsWindow::~GraphicsWindow() {
	if (ENABLE_VALIDATION_LAYERS) {
		destroyDebugUtilsMessengerExtension(instance, nullptr, debug_messenger);
	}

    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void GraphicsWindow::initializeWindow(int width, int height) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

void GraphicsWindow::createVulkanInstance() {

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
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
		.pNext = nullptr
    };
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;

	if (ENABLE_VALIDATION_LAYERS) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();

		debugMessengerPopulateCreateInfo(debug_create_info);
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
	}

	std::vector<const char*> extensions = getRequiredExtensions();
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

void GraphicsWindow::initializeGraphicsENV() {
    createVulkanInstance();
	setupDebugCallbackSys();
	pickPhysicalDevice();
}

int GraphicsWindow::rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures device_features;
	
	vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);

	//Create a score based on the device's properties.
	int score = 0;

	//Discrete GPUs have a significant performance advantage
	if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	//Maximum possible size of textures affects graphics quality
	score += device_properties.limits.maxImageDimension2D;

	return score;
}

void GraphicsWindow::pickPhysicalDevice() {
	uint32_t count;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);

	if (count == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());
	
	int highest_score = 0;
	VkPhysicalDevice best_device = VK_NULL_HANDLE;
	for (const auto& device : devices) {
		int score = rateDeviceSuitability(device);
		if (score > highest_score) {
			highest_score = score;
			best_device = device;
		}
	}

	if (best_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	physical_device = best_device;
}

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

void GraphicsWindow::setupDebugCallbackSys() {
	if (!ENABLE_VALIDATION_LAYERS) return;

	VkDebugUtilsMessengerCreateInfoEXT create_info;
	debugMessengerPopulateCreateInfo(create_info);

	if (createDebugUtilsMessengerExtension(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}

}

void GraphicsWindow::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

std::vector<const char*> GraphicsWindow::getRequiredExtensions() {
    uint32_t glfw_extension_count;
    const char** glfw_extensions =
        glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    #ifdef __APPLE__ 
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    if (ENABLE_VALIDATION_LAYERS) {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

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



VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsWindow::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
	VkDebugUtilsMessageTypeFlagsEXT message_type, 
	const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
	void *p_user_data
) {
	VkBool32 result = VK_FALSE;
	switch (message_severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			return VK_FALSE;
			std::cout << FAINT "[VERBOSE] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			return VK_FALSE;
			std::cout << GREEN_FG "[INFO] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cout << YELLOW_FG "[WARNING] " ANSI_NORMAL;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cout << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL;
			result = VK_TRUE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			std::cout << MAGENTA_FG_BRIGHT  "[UNKNOWN] " ANSI_NORMAL;
			break;
	}

	switch (message_type) {
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			std::cout << "[GENERAL]: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			std::cout << "[VALIDATION]: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			std::cout << "[PERFORMANCE]: ";
			break;
	}

	std::cout << p_callback_data->pMessage << std::endl;
	for (int i = 0; i < p_callback_data->objectCount; i++) {
		if (p_callback_data->pObjects[i].pObjectName) {
			std::cout << p_callback_data->pObjects[i].pObjectName << std::endl;
		}
	}

	return result;
    
}