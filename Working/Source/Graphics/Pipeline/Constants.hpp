#ifndef MEADOW_PIPELINE_CONSTANTS_HPP
#define MEADOW_PIPELINE_CONSTANTS_HPP

#include <vector>
#include <vulkan/vulkan.h>


namespace PipelineConstants {
    const std::vector<VkDynamicState> DYNAMIC_STATES {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
}

#endif // MEADOW_PIPELINE_CONSTANTS_HPP

