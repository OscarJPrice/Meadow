#pragma once
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "vulkan/vulkan_core.h"
#include <optional>
#include <vulkan/vulkan.h>

class VulkanSwapchain {
public:
  const VulkanDevice &device;

  VkSwapchainKHR vk_swapchain;
  VkExtent2D extent;
  VulkanRenderPass &render_pass;
  std::vector<VkImage> images;
  std::vector<VkImageView> image_views;
  std::optional<std::vector<VkFramebuffer>> frame_buffers;

  VulkanSwapchain(const VulkanDevice &device, VulkanRenderPass &render_pass);
  ~VulkanSwapchain();

  inline void recreate() {
    vkDeviceWaitIdle(device.vk_logical_device);
    destroy();
    create();
  }

private:
  std::vector<VkFramebuffer> createFrameBuffers(const VulkanDevice &device,
                                                VulkanSwapchain &swapchain,
                                                VulkanRenderPass &render_pass);
  void create();
  void destroy();

public:
  inline void setRenderPass(VulkanRenderPass &render_pass) {
    this->render_pass = render_pass;
    frame_buffers = createFrameBuffers(device, *this, render_pass);
  }
};
