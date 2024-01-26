#ifndef QUEUE_UTILS_HPP
#define QUEUE_UTILS_HPP

#include <optional>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    // Check if both graphics and present families are available
    inline bool isComplete() {
        return graphics_family.has_value() && present_family.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);


#endif // QUEUE_UTILS_HPP
