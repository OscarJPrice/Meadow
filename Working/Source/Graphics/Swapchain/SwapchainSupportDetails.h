#ifndef _MEADOW_SWAPCHAINSUPPORTDETAILS_H_
#define _MEADOW_SWAPCHAINSUPPORTDETAILS_H_

#include <vulkan/vulkan.h>
#include <vector>
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

#endif // _MEADOW_SWAPCHAINSUPPORTDETAILS_H_