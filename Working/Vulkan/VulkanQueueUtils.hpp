#pragma once
#include <optional>
#include <vulkan/vulkan.h>
#include "ModernTypes.h"
/**
 * @brief Namespace for queue utilities
 * 
 */
namespace QueueUtils {
    struct QueueFamilyIndices {
        std::optional<u32> graphics_family;
        std::optional<u32> present_family;

        /**
         * @brief Check if the queue families are complete
         * 
         * @return true if the queue families are complete
         * @return false if the queue families are not complete
         */
        inline bool isComplete() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice device, const VkSurfaceKHR surface);
}