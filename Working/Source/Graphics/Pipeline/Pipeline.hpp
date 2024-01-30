#ifndef _MEADOW_PIPELINE_HPP_
#define _MEADOW_PIPELINE_HPP_

#include <initializer_list>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "Viewport.hpp"
#include "Shader.hpp"

class Pipeline : Viewport {
    const GraphicsContext& graphics_context;
    Swapchain& swapchain;

    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;

    VkViewport viewport;

    const std::vector<Shader> shaders;

public:

    Pipeline(const GraphicsContext& graphics_context, 
        Swapchain& swapchain, 
        std::vector<Shader> shaders,
        bool blend = false);

    ~Pipeline();

private:

    void createPipelineLayout();


};

#endif