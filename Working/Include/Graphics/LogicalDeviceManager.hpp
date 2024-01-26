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

    LogicalDeviceManager(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

    ~LogicalDeviceManager();

    inline operator VkDevice() {
        return device;
    }
};

#endif // LOGICAL_DEVICE_MANAGER_HPP
