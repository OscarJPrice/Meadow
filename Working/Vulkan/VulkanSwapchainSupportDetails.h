#pragma once

#include <vector>
#include <vulkan/vulkan.h>
struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities{};
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;
};
