#ifndef MEADOW_PIPELINE_VIEWPORT_HPP
#define MEADOW_PIPELINE_VIEWPORT_HPP

#include <vulkan/vulkan.h>

class Viewport {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewport_state_create_info;

public:
    Viewport(VkExtent2D& extent);

    ~Viewport() = default;

    inline operator VkViewport&() { return viewport; }

    inline operator VkPipelineViewportStateCreateInfo*() { return &viewport_state_create_info; }
};

#endif