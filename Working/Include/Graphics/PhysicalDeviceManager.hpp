#ifndef MEADOW_PHYSICAL_DEVICE_MANAGER_HPP
#define MEADOW_PHYSICAL_DEVICE_MANAGER_HPP

#include <vulkan/vulkan.h>
#include "QueueUtils.hpp"

class PhysicalDeviceManager {
    VkPhysicalDevice device = VK_NULL_HANDLE;

public:
    PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface);

    ~PhysicalDeviceManager() = default;

    inline operator VkPhysicalDevice() {
        return device;
    }

private:
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    uint32_t rateDeviceSuitability(VkPhysicalDevice device);

};

#endif // PHYSICAL_DEVICE_MANAGER_HPP