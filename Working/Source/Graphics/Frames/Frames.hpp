#ifndef _MEADOW_Frames_HPP_
#define _MEADOW_Frames_HPP_

#include <vulkan/vulkan.h>
#include <vector>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "CommandPool.hpp"


/**
 * @brief The Frames class represents a collection of frames used for rendering graphics.
 * 
 * This class manages synchronization objects, such as semaphores and fences, 
 * as well as command pools for each frame. It provides functionality to draw a frame.
 */
class Frames {
    const GraphicsContext& context;
    Swapchain& swapchain;
    Pipeline& pipeline;

    std::vector<VkSemaphore> image_available;
    std::vector<VkSemaphore> render_finished;
    std::vector<VkFence> frame_rendered_fence;
    std::vector<CommandPool> command_pools;
    uint32_t current_frame;

public:
    Frames(const GraphicsContext& device, Swapchain& swapchain, Pipeline& pipeline);

    ~Frames();

    void drawFrame(); //Draw the bloody frame to the screen!
    
private:
    void createSyncObjs();

    void resetSyncObjs();

    void cleanup();


};





#endif