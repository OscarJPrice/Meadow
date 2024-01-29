#ifndef LOGICAL_DEVICE_MANAGER_HPP
#define LOGICAL_DEVICE_MANAGER_HPP

#include <vulkan/vulkan.h>

/**
 * @brief Wrapper class for the logical device
 * 
 */
class LogicalDeviceManager {
    VkDevice device = VK_NULL_HANDLE;
    VkQueue present_queue = VK_NULL_HANDLE;
public:

    LogicalDeviceManager(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface);

    ~LogicalDeviceManager();

    inline operator const VkDevice&() const {
        return device;
    }

    inline operator VkQueue&() {
        return present_queue;
    }

    inline operator const VkQueue() const {
        return present_queue;
    }

};

#endif // LOGICAL_DEVICE_MANAGER_HPP
