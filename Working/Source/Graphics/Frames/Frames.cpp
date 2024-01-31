#include "Frames.hpp"
#include "Config.h"
#include <stdexcept>
#include <iostream>


Frames::Frames(const GraphicsContext& context, Swapchain& swapchain, 
    Pipeline& pipeline) : 
    context(context), swapchain(swapchain), pipeline(pipeline), current_frame(0)
{
    command_pools.reserve(CONSTANTS::FRAMES_IN_FLIGHT);
    for (uint32_t i = 0; i < CONSTANTS::FRAMES_IN_FLIGHT; i++) {
        command_pools.emplace_back(context, swapchain);
        command_pools[i].createCommandBuffer();
    }
    createSyncObjs();
}

Frames::~Frames() {
    vkDeviceWaitIdle(context.getLogicalDevice());
    cleanup();
}

void Frames::cleanup() {
    for (uint32_t i = 0; i < CONSTANTS::FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(context.getLogicalDevice(), image_available[i], nullptr);
        vkDestroySemaphore(context.getLogicalDevice(), render_finished[i], nullptr);
        vkDestroyFence(context.getLogicalDevice(), frame_rendered_fence[i], nullptr);
    }
}

void Frames::createSyncObjs() {
    image_available.resize(CONSTANTS::FRAMES_IN_FLIGHT);
    render_finished.resize(CONSTANTS::FRAMES_IN_FLIGHT);
    frame_rendered_fence.resize(CONSTANTS::FRAMES_IN_FLIGHT);
    VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        
    };

    VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    for (uint32_t i = 0; i < CONSTANTS::FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(context.getLogicalDevice(), &semaphore_create_info, nullptr, 
                &image_available[i]) ||
            vkCreateSemaphore(context.getLogicalDevice(), &semaphore_create_info, nullptr, 
                &render_finished[i]) ||
            vkCreateFence(context.getLogicalDevice(), &fence_create_info, nullptr, 
                &frame_rendered_fence[i])) 
        {
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

void Frames::drawFrame() {
    vkWaitForFences(context.getLogicalDevice(), 1, 
        &frame_rendered_fence[current_frame], VK_TRUE, UINT64_MAX);

    vkResetFences(context.getLogicalDevice(), 1, &frame_rendered_fence[current_frame]);

    uint32_t image_index;
    vkAcquireNextImageKHR(context.getLogicalDevice(), swapchain, UINT64_MAX, 
        image_available[current_frame], VK_NULL_HANDLE, &image_index);
    
    vkResetCommandBuffer(command_pools[current_frame].getCommandBuffer(0), 0);
    command_pools[current_frame].beginCommandBuffer(0, image_index, pipeline);

    const VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &image_available[current_frame],
        .pWaitDstStageMask = &wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_pools[current_frame].getCommandBuffer(0),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &render_finished[current_frame]
    };

    if (vkQueueSubmit(context.getPresentQueue(), 1, &submit_info, frame_rendered_fence[current_frame])) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }
    
    VkPresentInfoKHR present_info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &render_finished[current_frame],
        .swapchainCount = 1,
        .pSwapchains = swapchain,
        .pImageIndices = &image_index
    };

    vkQueuePresentKHR(context.getPresentQueue(), &present_info);
    current_frame = (current_frame + 1) * (current_frame+1 < CONSTANTS::FRAMES_IN_FLIGHT);

    
}