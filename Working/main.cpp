// #include "Vulkan-Instance.hpp"
#include "Shaders.hpp"
#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanSwapchain.hpp"

int main() {
  VulkanInstance instance;
  VulkanSurface surface(instance);
  const VulkanDevice device(instance, surface);
  Shaders shaders(device);
  VulkanRenderPass pass(device);
  VulkanSwapchain swapchain(device, pass);
#pragma unroll
  while (surface) {
  }
}
