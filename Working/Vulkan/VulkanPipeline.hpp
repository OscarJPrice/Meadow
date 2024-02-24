#pragma once
#include "Shaders.hpp"
#include "VulkanSwapchain.hpp"

namespace PipelineUtils {

inline std::vector<VkPipelineShaderStageCreateInfo>
AcquireShaderStages(const char *entry_point, const auto &...shaders) {
  return {shaders.shader_stage_info(entry_point)...};
}

inline VkViewport makeViewport(const VkExtent2D &extent) {
  return {.x = 0.0f,
          .y = 0.0f,
          .width = static_cast<float>(extent.width),
          .height = static_cast<float>(extent.height),
          .minDepth = 0.0f,
          .maxDepth = 1.0f};
}

inline VkRect2D makeScissor(const VkExtent2D &extent) {
  return {.offset = {0, 0}, .extent = extent};
}

} // namespace PipelineUtils

class VulkanPipeline {
public:
  const VulkanSwapchain &swapchain;

  VkPipeline vk_pipeline;
  VkPipelineLayout pipeline_layout;
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
  bool blend;

  VkViewport viewport;
  VkRect2D scissor;

  VulkanPipeline(const VulkanSwapchain &swapchain,
                 std::vector<VkPipelineShaderStageCreateInfo> shader_stages,
                 bool blend = true);

  ~VulkanPipeline();

  inline void recreate() {
    vkDeviceWaitIdle(swapchain.device.vk_logical_device);
    destroy();
    create();
  }

private:
  void create();
  void destroy();
};
