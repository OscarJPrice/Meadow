#ifndef FLYING_FRAMES_HPP
#define FLYING_FRAMES_HPP
#include <vulkan/vulkan.h>
#include <vector>
#include "DrawingConstants.hpp"
#include "CommandBuffer.hpp"

class FlyingFrames {
    std::vector<CommandBuffer> command_buffers;
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> frame_rendered_fences;

public:

    FlyingFrames();

    ~FlyingFrames();

    void drawFrame();

};

#endif // FLYING_FRAMES_HPP
