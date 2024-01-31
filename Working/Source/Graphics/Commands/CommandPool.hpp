#ifndef _MEADOW_COMMAND_POOL_HPP
#define _MEADOW_COMMAND_POOL_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"

class CommandPool {
    VkCommandPool command_pool;
    const GraphicsContext& context;
    Swapchain& swapchain;
    std::vector<VkCommandBuffer> command_buffers;

public:
    CommandPool(const GraphicsContext& context, Swapchain& swapchain);

    ~CommandPool();

    void createCommandBuffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    void beginCommandBuffer(uint32_t command_buffer, uint32_t image_index, Pipeline& pipeline);

    void draw();
};


#endif