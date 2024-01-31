#include "CommandPool.hpp"
#include "QueueUtils.hpp"

CommandPool::CommandPool(const GraphicsContext& context, Swapchain& swapchain) 
    : context(context), swapchain(swapchain)
{

    QueueUtils::QueueFamilyIndices queue_family_indices = 
        QueueUtils::findQueueFamilies(context.getPhysicalDevice(), context.getSurface());
    
    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queue_family_indices.graphics_family.value()
    };

    if (vkCreateCommandPool(context.getLogicalDevice(), &pool_info, nullptr, &command_pool)) {
        throw std::runtime_error("Failed to create command pool!");
    }   

}

CommandPool::~CommandPool() {
    for (VkCommandBuffer command_buffer : command_buffers) {
        vkFreeCommandBuffers(context.getLogicalDevice(), command_pool, 1, &command_buffer);
    }

    vkDestroyCommandPool(context.getLogicalDevice(), command_pool, nullptr);
}

void CommandPool::createCommandBuffer(VkCommandBufferLevel level) {
    VkCommandBuffer command_buffer;

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = level,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(context.getLogicalDevice(), &alloc_info, &command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    command_buffers.push_back(command_buffer);
}

void CommandPool::beginCommandBuffer(uint32_t command_buffer, uint32_t image_index, Pipeline& pipeline) {
    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    if (vkBeginCommandBuffer(command_buffers[command_buffer], &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo render_pass_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = swapchain.getRenderPass(),
        .framebuffer = swapchain.getFramebuffers()[image_index],
        .renderArea = {
            .offset = {0, 0},
            .extent = swapchain.getExtent()
        }
    };

    vkCmdBeginRenderPass(command_buffers[command_buffer], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffers[command_buffer], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    vkCmdSetViewport(command_buffers[command_buffer], 0, 1, &pipeline.getViewport());

    vkCmdSetScissor(command_buffers[command_buffer], 0, 1, &pipeline.getScissor());

    vkCmdDraw(command_buffers[command_buffer], 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffers[command_buffer]);

    if (vkEndCommandBuffer(command_buffers[command_buffer]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}