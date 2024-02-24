// #include "Vulkan-Instance.hpp"
#include "Shaders.hpp"
#include "VulkanCommand.hpp"
#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanSurface.hpp"
#include "VulkanSwapchain.hpp"

int main() {
  VulkanInstance instance;
  VulkanSurface surface(instance);
  const VulkanDevice device(instance, surface);
  Shaders::loadShaders(device);
  VulkanSwapchain swapchain(device);
  VulkanPipeline pipeline(
      swapchain, PipelineUtils::AcquireShaderStages(
                     "main", Shaders::get("vert"), Shaders::get("frag")));
  VulkanCommandPool command_pool(swapchain);
  command_pool.addCommandBuffer();
  std::cout << command_pool.command_buffers.size() << std::endl;
#pragma unroll
  while (surface) {
    command_pool.command_buffers[0](pipeline);
  }
  Shaders::cleanup();
}
