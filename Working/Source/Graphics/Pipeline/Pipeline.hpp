#ifndef PIPELINE_HPP
#define PIPELINE_HPP

// Include necessary headers here
#include <vector>
#include <vulkan/vulkan.h>
#include "Shader.hpp"
#include "PipelineConstants.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"
#include "Viewport.hpp"

class Pipeline {
    const VkDevice& device;
    Shader vert_shader_module;
    Shader frag_shader_module;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;
    Viewport viewport;
    RenderPass render_pass;

    VkPipelineShaderStageCreateInfo shader_stages[2];

public:
    Pipeline(const VkDevice& device, const SwapChain& swapchain, const bool blend = false);

    ~Pipeline();

    inline operator VkPipelineLayout&() { return pipeline_layout; }

    inline operator VkPipeline&() { return pipeline; }

    inline operator VkRenderPass&() { return render_pass; }

    inline operator const VkRect2D&() { return viewport; }

    inline operator VkRect2D*() { return viewport; }

    inline operator const VkViewport&() { return viewport; }

    inline operator VkViewport*() { return viewport; }


};

#endif // PIPELINE_HPP
