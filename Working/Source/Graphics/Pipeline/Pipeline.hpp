#ifndef PIPELINE_HPP
#define PIPELINE_HPP

// Include necessary headers here
#include <vector>
#include <vulkan/vulkan.h>
#include "Shader.hpp"
#include "Constants.hpp"

class Pipeline {
    VkDevice& device;
    Shader vert_shader_module;
    Shader frag_shader_module;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;

    VkPipelineShaderStageCreateInfo shader_stages[2];

public:
    Pipeline(VkDevice& device, VkExtent2D& swapchain, bool blend = false);

    ~Pipeline();

    inline operator VkPipelineLayout&() { return pipeline_layout; }
};

#endif // PIPELINE_HPP
