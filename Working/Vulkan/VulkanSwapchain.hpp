#pragma once
#include "VulkanDevice.hpp"
#include "VulkanSurface.hpp"
#include <vulkan/vulkan.h>

class VulkanSwapchain {
public:
  const VulkanDevice &device;

  VkSwapchainKHR vk_swapchain;
  VkExtent2D extent;

  VkSurfaceFormatKHR surface_format;
  std::vector<VkImage> images;
  std::vector<VkImageView> image_views;

  VulkanSwapchain(const VulkanDevice &device);
  ~VulkanSwapchain();

  inline void recreate() {
    vkDeviceWaitIdle(device.vk_logical_device);
    destroy();
    create();
  }

private:
  void create();
  void destroy();
};
