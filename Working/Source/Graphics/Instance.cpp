#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "Graphics/Instance.hpp"
#include "Logging/Logging.hpp"
#include "ansi.h"
#include "Graphics/DebugConstants.hpp"


Instance::Instance(const char* name) {
    Log::init(); //initialize logging - VERY IMPORTANT
    {
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

        if (DEBUG_MODE) {
            create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();

            debugMessengerPopulateCreateInfo(debug_create_info);
            create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
        }

        if (!checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
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
    {
        if (!DEBUG_MODE) return;

        VkDebugUtilsMessengerCreateInfoEXT create_info;
        debugMessengerPopulateCreateInfo(create_info);

        if (createDebugUtilsMessengerExtension(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug callback!");
        }
    }
}

Instance::~Instance() {
    if (DEBUG_MODE) {
        destroyDebugUtilsMessengerExtension(instance, nullptr, debug_messenger);
    }
    vkDestroyInstance(instance, nullptr);
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
std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfw_extension_count;
    const char** glfw_extensions =
        glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    #ifdef __APPLE__ 
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif

    if (DEBUG_MODE) {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/**
 * Checks if the given extensions are supported by the graphics window.
 * 
 * @param extensions A vector of extension names to check for support.
 * @return True if all extensions are supported, false otherwise.
 */
bool Instance::checkExtensionsSupport(std::vector<const char*> extensions) {
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
 * @brief Checks if the required validation layers are supported by the Vulkan instance.
 * 
 * @return true if all validation layers are supported, false otherwise.
 */
bool Instance::checkValidationLayerSupport()
{

    std::vector<VkLayerProperties> available_layers;
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        available_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 
    }

    //this is used in place of std::includes because it allows for debug info.
    for (const char* layer : VALIDATION_LAYERS) {
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
 * @brief Populates the given VkDebugUtilsMessengerCreateInfoEXT struct with the necessary information for the debug messenger.
 * 
 * @param create_info The VkDebugUtilsMessengerCreateInfoEXT struct to populate.
 */
void Instance::debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
	create_info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
        .pUserData = nullptr
    };
}

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
VkResult Instance::createDebugUtilsMessengerExtension(
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
VkResult Instance::destroyDebugUtilsMessengerExtension(
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

/**
 * @brief Debug callback function for the GraphicsWindowTotal class.
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
VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(
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
            stream = &Log::unexpected;
            *stream << MAGENTA_FG_BRIGHT "[UNKNOWN] " ANSI_NORMAL;
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