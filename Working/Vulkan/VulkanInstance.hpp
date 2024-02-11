#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "DebugCallback.hpp"
#include "Config.h"
#include <iostream>

inline std::vector<const char*> getRequiredExtensions() {
    glfwInit();
    // Get the required instance extensions using glfwGetRequiredInstanceExtensions()
    uint32_t glfw_extension_count;
    const char** glfw_extensions =
        glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    #ifdef __APPLE__ 
        // Add the VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME extension for Apple platforms
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif

    if (CONSTANTS::DEBUG_MODE) {
        // Add the VK_EXT_DEBUG_UTILS_EXTENSION_NAME extension if in debug mode
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // Return the vector of required extensions
    return extensions;
}
const std::vector<const char*> extensions = getRequiredExtensions();

const static VkApplicationInfo applicationInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = nullptr,
    .pApplicationName = CONSTANTS::APP_NAME,
    .applicationVersion = CONSTANTS::ENGINE_VERSION,
    .pEngineName = CONSTANTS::ENGINE_NAME,
    .engineVersion = CONSTANTS::ENGINE_VERSION,
    .apiVersion = VK_API_VERSION_1_3
};

const static VkInstanceCreateInfo instanceCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = CONSTANTS::DEBUG_MODE ? &DebugCallback::create_info : nullptr,
    .flags = CONSTANTS::INSTANCE_FLAGS,
    .pApplicationInfo = &applicationInfo,
    .enabledLayerCount = CONSTANTS::VALIDATION_LAYERS.size(),
    .ppEnabledLayerNames = CONSTANTS::VALIDATION_LAYERS.data(),
    .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data()
};


struct VulkanInstance {
    
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT debug_messenger;

    VulkanInstance();

    ~VulkanInstance();
};