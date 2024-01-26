#ifndef MEADOW_PHYSICAL_DEVICE_MANAGER_HPP
#define MEADOW_PHYSICAL_DEVICE_MANAGER_HPP

#include <vulkan/vulkan.h>
#include "QueueUtils.hpp"
#include "Graphics/SwapChain.hpp"


/**
 * @brief Wrapper class for the physical device
 * 
 */
class PhysicalDeviceManager {
    VkPhysicalDevice device = VK_NULL_HANDLE;

public:
    PhysicalDeviceManager(const VkInstance& instance, const VkSurfaceKHR& surface);

    ~PhysicalDeviceManager() = default;

    inline operator VkPhysicalDevice&() {
        return device;
    }

private:
    bool isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

    static uint32_t rateDeviceSuitability(const VkPhysicalDevice& device);

    static bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);

public:

    static SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

};

#endif // PHYSICAL_DEVICE_MANAGER_HPP