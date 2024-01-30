#ifndef MEADOW_PIPELINE_VIEWPORT_HPP
#define MEADOW_PIPELINE_VIEWPORT_HPP

#include <vulkan/vulkan.h>

class Viewport {

protected:
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewport_state_create_info;

public:
    Viewport(const VkExtent2D& extent);

    ~Viewport() = default;
};

#endif