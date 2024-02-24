#pragma once
#include <vector>
#define VK_ENABLE_BETA_EXTENSIONS
#include "VulkanPipeline.hpp"
#include "VulkanSwapchain.hpp"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class VulkanCommandPool;

class VulkanCommandBuffer {
public:
  const VulkanCommandPool &command_pool;

  VkCommandBuffer vk_command_buffer;

  VulkanCommandBuffer(
      const VulkanCommandPool &command_pool, int n = 1,
      VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  ~VulkanCommandBuffer();
  VulkanCommandBuffer(VulkanCommandBuffer &&other) noexcept;

  void operator()(VulkanPipeline &pipeline) const;
};

class VulkanCommandPool {
public:
  VulkanSwapchain &swapchain;
  VkCommandPool vk_command_pool;
  std::vector<VulkanCommandBuffer> command_buffers;

  VulkanCommandPool(VulkanSwapchain &swapchain);
  ~VulkanCommandPool();

  inline void addCommandBuffer(
      int n = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
    command_buffers.emplace_back(*this, n, level);
  }
};
