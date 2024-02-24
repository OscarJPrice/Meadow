#pragma once

#include <vulkan/vulkan.h>

namespace DebugCallback {
VkResult create(const VkInstance &instance,
                const VkAllocationCallbacks *allocator,
                VkDebugUtilsMessengerEXT &callback);

VkResult destroy(const VkInstance &instance,
                 const VkAllocationCallbacks *allocator,
                 VkDebugUtilsMessengerEXT &callback);

VKAPI_ATTR VkBool32 VKAPI_CALL callable(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

const VkDebugUtilsMessengerCreateInfoEXT create_info = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = DebugCallback::callable,
    .pUserData = nullptr};
} // namespace DebugCallback
