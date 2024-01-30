#ifndef MEADOW_COMMAND_BUFFERS_HPP
#define MEADOW_COMMAND_BUFFERS_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include "DrawingConstants.hpp"
#include "SwapChain.hpp"
#include "Pipeline.hpp"
#include "LogicalDeviceManager.hpp"

template<uint32_t N = DrawingConstants::FRAMES_IN_FLIGHT >
class CommandBuffers {
    std::vector<VkCommandBuffer> command_buffers;
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> frame_rendered_fence;
    uint32_t current_frame = 0;

    const LogicalDeviceManager& logical_device;
    // const VkQueue& drawing_queue;
    SwapChain& swap_chain;
    Pipeline& pipeline;
    const std::vector<VkFramebuffer>& frame_buffers;
    const VkCommandPool& command_pool;

public:
    
    CommandBuffers( const LogicalDeviceManager& device, SwapChain& swap_chain,
        Pipeline& pipeline, const std::vector<VkFramebuffer>& frame_buffers,
        const VkCommandPool& command_pool) :
        
        logical_device(device),
        // drawing_queue(QueueUtils::get_graphics_queue(device)),
        swap_chain(swap_chain),
        pipeline(pipeline),
        frame_buffers(frame_buffers),
        command_pool(command_pool)
    {
        command_buffers.resize(N);
        image_available_semaphores.resize(N);
        render_finished_semaphores.resize(N);
        frame_rendered_fence.resize(N);
        initialize_sync_objects();

        VkCommandBufferAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = (uint32_t)command_buffers.size()
        };

        if (vkAllocateCommandBuffers(device, &alloc_info, command_buffers.data())) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

    }

    ~CommandBuffers() {
        vkFreeCommandBuffers(logical_device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
        for (uint32_t i = 0; i < N; i++) {
            vkDestroySemaphore(logical_device, image_available_semaphores[i], nullptr);
            vkDestroySemaphore(logical_device, render_finished_semaphores[i], nullptr);
            vkDestroyFence(logical_device, frame_rendered_fence[i], nullptr);
        }
    }

    VkCommandBuffer& operator[](uint32_t index) {
        if (index >= command_buffers.size()) {
            throw std::runtime_error("Command buffer index out of range!");
        }
        return command_buffers[index];
    }

    inline void initialize_sync_objects() {
        VkSemaphoreCreateInfo semaphore_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        VkFenceCreateInfo fence_create_info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        for (uint32_t i = 0; i < N; i++) {
            if (vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, 
                    &image_available_semaphores[i]) ||
                vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, 
                    &render_finished_semaphores[i]) ||
                vkCreateFence(logical_device, &fence_create_info, nullptr, 
                    &frame_rendered_fence[i])) 
            {
                throw std::runtime_error("Failed to create synchronization objects for a frame!");
            }
        }
    }

    void record(uint32_t command_buffer_index, uint32_t image_index) {
        VkCommandBufferBeginInfo begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
        };

        if (vkBeginCommandBuffer(command_buffers[command_buffer_index], &begin_info)) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        VkRenderPassBeginInfo render_pass_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = (VkRenderPass)pipeline,
            .framebuffer = frame_buffers[image_index],
            .renderArea = {
                .offset = {0, 0},
                .extent = swap_chain.extent()
            },
            .clearValueCount = 1,
            .pClearValues = &clear_color
        };  

        vkCmdBeginRenderPass(command_buffers[command_buffer_index], 
            &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(command_buffers[command_buffer_index], 
            VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        vkCmdSetViewport(command_buffers[command_buffer_index], 0, 1, pipeline);

        vkCmdSetScissor(command_buffers[command_buffer_index], 0, 1, pipeline);

        vkCmdDraw(command_buffers[command_buffer_index], 3, 1, 0, 0);

        vkCmdEndRenderPass(command_buffers[command_buffer_index]);

        if (vkEndCommandBuffer(command_buffers[command_buffer_index])) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void reset(uint32_t command_buffer_index = std::numeric_limits<uint32_t>::max()) {
        if (command_buffer_index == std::numeric_limits<uint32_t>::max()) {
            for (auto& command_buffers : command_buffers) {
                vkResetCommandBuffer(command_buffers, 0);
            }
        }
        else if (command_buffer_index >= command_buffers.size()) {
            throw std::runtime_error("Command buffer index out of range!");
        }
        else {
            vkResetCommandBuffer(command_buffers[command_buffer_index], 0);
        }
    }

    void drawFrame() {
        vkWaitForFences(logical_device, 1, &frame_rendered_fence[current_frame], 
            VK_TRUE, UINT64_MAX);
        vkResetFences(logical_device, 1, &frame_rendered_fence[current_frame]);
        uint32_t image_index;
        vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX, 
            image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        record(current_frame, image_index);

        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &image_available_semaphores[current_frame],
            .pWaitDstStageMask = wait_stages,
            .commandBufferCount = 1,//(uint32_t)command_buffers.size(),
            .pCommandBuffers = &command_buffers[current_frame],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &render_finished_semaphores[current_frame]
        };

        if (vkQueueSubmit(logical_device.queue(), 1, &submit_info,
            frame_rendered_fence[current_frame])) 
        {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &render_finished_semaphores[current_frame],
            .swapchainCount = 1,
            .pSwapchains = swap_chain,
            .pImageIndices = &image_index
        };

        vkQueuePresentKHR(logical_device.queue(), &present_info);
        current_frame = (current_frame + 1) * (current_frame+1 < N);
    }
};

#endif