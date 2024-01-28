#include "Viewport.hpp"

Viewport::Viewport(VkExtent2D& extent) : 
    viewport({
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)(extent.width),
        .height = (float)(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    }), 
    scissor({
        .offset = {0, 0},
        .extent = extent
    }),
    viewport_state_create_info({
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    })
{
}