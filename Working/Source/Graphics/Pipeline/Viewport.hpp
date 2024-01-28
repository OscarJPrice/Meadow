#ifndef MEADOW_PIPELINE_VIEWPORT_HPP
#define MEADOW_PIPELINE_VIEWPORT_HPP

#include <vulkan/vulkan.h>

class Viewport {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewport_state_create_info;

public:
    Viewport(const VkExtent2D& extent);

    ~Viewport() = default;

    inline operator const VkViewport&() { return viewport; }

    inline operator VkViewport*() { return &viewport; }

    inline operator const VkRect2D&() { return scissor; }

    inline operator VkRect2D*() { return &scissor; }

    inline operator VkPipelineViewportStateCreateInfo*() { return &viewport_state_create_info; }
};

#endif