//#include "Vulkan-Instance.hpp"
#include "VulkanInstance.hpp"
#include "VulkanDevice.hpp"
#include "Shaders.hpp"
#include "VulkanSurface.hpp"
#include "VulkanSwapchain.hpp"

int main() {
    VulkanInstance instance;
    VulkanSurface surface(instance);
    const VulkanDevice device(instance, surface);
    Shaders::loadShaders(device);
    VulkanSwapchain swapchain(device, surface);

    #pragma unroll
    while (surface) {
        // Do NOTHING!
    }
    Shaders::cleanup();
}