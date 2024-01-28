#include "Application.hpp"

Application::Application(uint32_t width, uint32_t height, const char* name) :
    window(width, height, name), 
    instance(name),
    surface(instance, window),
    physical_device(instance, surface), 
    logical_device(physical_device, surface), 
    swap_chain(window, physical_device, logical_device, surface), 
    image_view(logical_device, swap_chain), 
    pipeline(logical_device, swap_chain), 
    frame_buffer(logical_device, swap_chain, image_view, pipeline), 
    command_pool(surface, physical_device, logical_device),
    command_buffer(command_pool, logical_device, pipeline, frame_buffer, swap_chain.extent())
{
    create_sync_objects();
    run();
}

Application::~Application() {
    vkDestroyFence(logical_device, unique_frame_rendered_fence, nullptr);
    vkDestroySemaphore(logical_device, render_finished_semaphore, nullptr);
    vkDestroySemaphore(logical_device, image_available_semaphore, nullptr);
}

void Application::run(){
    while(window){
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(logical_device);
}

void Application::drawFrame() {
    vkWaitForFences(logical_device, 1, &unique_frame_rendered_fence, 
        VK_TRUE, UINT64_MAX);
    vkResetFences(logical_device, 1, &unique_frame_rendered_fence);
    uint32_t image_index;
    vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX, 
        image_available_semaphore, VK_NULL_HANDLE, &image_index);

    command_buffer.reset();
    command_buffer.record(image_index);

    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &image_available_semaphore,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = command_buffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &render_finished_semaphore
    };

    if (vkQueueSubmit(logical_device, 1, &submit_info, 
            unique_frame_rendered_fence)) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR present_info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &render_finished_semaphore,
        .swapchainCount = 1,
        .pSwapchains = swap_chain,
        .pImageIndices = &image_index
    };

    vkQueuePresentKHR(logical_device, &present_info);
}

void Application::create_sync_objects() {
    VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if (vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, 
            &image_available_semaphore) ||
        vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, 
            &render_finished_semaphore) ||
        vkCreateFence(logical_device, &fence_create_info, nullptr, 
            &unique_frame_rendered_fence)
    ) {
        throw std::runtime_error("Failed to create synchronization objects for a frame!");
    }
}