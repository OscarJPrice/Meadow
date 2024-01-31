#ifndef _MEADOW_PIPELINE_HPP_
#define _MEADOW_PIPELINE_HPP_

#include <initializer_list>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "Viewport.hpp"
#include "Shader.hpp"

class Pipeline : public Viewport {
    const GraphicsContext& graphics_context;
    Swapchain& swapchain;

    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;


    const std::vector<Shader> shaders;

public:

    Pipeline(const GraphicsContext& graphics_context, 
        Swapchain& swapchain, 
        std::vector<Shader> shaders,
        bool blend = false);

    ~Pipeline();

    inline operator VkPipeline&() { return pipeline; }

    inline VkViewport& getViewport() { return viewport; }

    inline VkRect2D& getScissor() { return scissor; }

private:

    void createPipelineLayout();


};

#endif