#pragma once
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanSwapchainSupportDetails.h"
#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>

class VulkanDevice {
public:
  const VulkanSurface &surface;

  VkPhysicalDevice vk_physical_device;
  VkDevice vk_logical_device;
  VkQueue present_queue;

  VulkanDevice(VulkanInstance &instance, VulkanSurface &surface);
  ~VulkanDevice();

  static SwapchainSupportDetails
  querySwapchainSupport(const VkPhysicalDevice &device,
                        const VkSurfaceKHR &surface);

  inline SwapchainSupportDetails querySwapchainSupport() const {
    return querySwapchainSupport(vk_physical_device, surface.vk_surface);
  };

  VkSurfaceFormatKHR chooseSurfaceFormat() const;
};
