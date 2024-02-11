#pragma once
#include <vulkan/vulkan.h>
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"

class VulkanDevice {
public:
    VkPhysicalDevice physical_device;

    VkDevice logical_device;
    VkQueue present_queue;

    VulkanDevice(VulkanInstance& instance, VulkanSurface& surface);
    ~VulkanDevice();

};