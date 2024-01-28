#ifndef QUEUE_UTILS_HPP
#define QUEUE_UTILS_HPP

#include <optional>
#include <vulkan/vulkan.h>


/**
 * @brief Namespace for queue utilities
 * 
 */
namespace QueueUtils {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

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

#endif // QUEUE_UTILS_HPP
