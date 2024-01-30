#ifndef COMMANDPOOL_HPP
#define COMMANDPOOL_HPP

#include <vulkan/vulkan.h>

class CommandPool {
    VkCommandPool command_pool;
    const VkDevice& device;
public:
    CommandPool(const VkSurfaceKHR& surface, const VkPhysicalDevice& physical_device, 
        const VkDevice& device);
    ~CommandPool();

    inline operator const VkCommandPool&() const { return command_pool; }
};

#endif // COMMANDPOOL_HPP
