#include <cstring>
#include <iostream>
#include "VulkanInstance.hpp"
#include "Debug-Macros.hpp"
#include "Config.h"


//forward declarations for tidyness
bool checkValidationLayerSupport();
bool checkExtensionsSupport(const std::vector<const char*>& extensions);

VulkanInstance::VulkanInstance() {

    ATTEMPT_BOOL(!CONSTANTS::DEBUG_MODE || checkValidationLayerSupport());

    ATTEMPT_BOOL(checkExtensionsSupport(extensions));

    ATTEMPT_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &vk_instance));

    if (CONSTANTS::DEBUG_MODE) {
        ATTEMPT_VK(DebugCallback::create(vk_instance, nullptr, 
            debug_messenger));
    }
}


VulkanInstance::~VulkanInstance()
{
    if (CONSTANTS::DEBUG_MODE) {
        DebugCallback::destroy(vk_instance, nullptr, debug_messenger);
    }
    vkDestroyInstance(vk_instance, nullptr);
    glfwTerminate();
}

/**
 * @brief Checks if the required validation layers are supported by the Vulkan instance.
 * 
 * @return true if all validation layers are supported, false otherwise.
 */
bool checkValidationLayerSupport() {

    // Retrieve the available instance layers
    std::vector<VkLayerProperties> available_layers;
    {
        u32 layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        available_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 
    }

    // Check if the required validation layers are supported
    // This loop checks if each validation layer in EnvConstants::VALIDATION_LAYERS is present in the available_layers
    // If a validation layer is found, it continues to the next layer
    // If a validation layer is not found, it returns false
    for (const char* layer : CONSTANTS::VALIDATION_LAYERS) {
        for (const VkLayerProperties& available_layer : available_layers) {
            if (strcmp(layer, available_layer.layerName) == 0) {
                goto next;
            }
        }
        return false;
        next: continue;
    }

    // All validation layers are supported, return true
    return true;
}

bool checkExtensionsSupport(const std::vector<const char*>& extensions) {

    // Retrieve the available instance extensions
    std::vector<VkExtensionProperties> available_extensions;
    {
        u32 extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        available_extensions.resize(extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
    }

    // Check if the required extensions are supported
    for (const char* extension : extensions) {
        for (const auto& available_extension : available_extensions) {
            if (strcmp(extension, available_extension.extensionName) == 0) {
                // Extension is supported, continue to the next extension
                goto next;
            }
        }
        // Extension is not supported, return false
        return false;
        next: continue;
    }

    // All extensions are supported, return true
    return true;

}