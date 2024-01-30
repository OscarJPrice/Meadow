#include "CommandPool.hpp"

#include <stdexcept>
#include "QueueUtils.hpp"

CommandPool::CommandPool(const VkSurfaceKHR& surface, 
    const VkPhysicalDevice& physical_device, const VkDevice& device)
     : device(device) 
{
    QueueUtils::QueueFamilyIndices queue_family_indices = 
        QueueUtils::findQueueFamilies(physical_device, surface);
    
    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queue_family_indices.graphics_family.value()
    };

    if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool)) {
        throw std::runtime_error("Failed to create command pool!");
    }   
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(device, command_pool, nullptr);
}