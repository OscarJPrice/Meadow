#include "CommandBuffer.hpp"
#include <stdexcept>

CommandBuffer::CommandBuffer(const VkCommandPool& command_pool, const VkDevice& device, 
    Pipeline& pipeline, const std::vector<VkFramebuffer>& frame_buffers, 
    const VkExtent2D& swap_chain_extent) :
    command_pool(command_pool),
    device(device),
    pipeline(pipeline),
    frame_buffers(frame_buffers),
    swap_chain_extent(swap_chain_extent)
{
    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(device, &alloc_info, &command_buffer)) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
}

void CommandBuffer::record(uint32_t image_index) {
    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    if (vkBeginCommandBuffer(command_buffer, &begin_info)) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkRenderPassBeginInfo render_pass_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = (VkRenderPass)pipeline,
        .framebuffer = frame_buffers[image_index],
        .renderArea = {
            .offset = {0, 0},
            .extent = swap_chain_extent
        },
        .clearValueCount = 1,
        .pClearValues = &clear_color
    };

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    vkCmdSetViewport(command_buffer, 0, 1, pipeline);

    vkCmdSetScissor(command_buffer, 0, 1, pipeline);

    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer)) {
        throw std::runtime_error("Failed to record command buffer!");
    }

}

void CommandBuffer::reset(uint32_t image_index) {
    vkResetCommandBuffer(command_buffer, 0);
}

