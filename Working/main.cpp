//#include "Vulkan-Instance.hpp"
#include <vulkan/vulkan.h>
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanDevice.hpp"
#include "Shaders.hpp"
//#include "CompileShader.hpp"

int main() {

    VulkanInstance instance;
    VulkanSurface surface(instance);
    VulkanDevice device(instance, surface);
    Shaders::loadShaders(device);
    while (surface) {
        // Do NOTHING!
    }
    Shaders::cleanup();
    return 0;
}