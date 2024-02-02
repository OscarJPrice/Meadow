#ifndef _MEADOW_PIPELINE_HPP_
#define _MEADOW_PIPELINE_HPP_

#include <initializer_list>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "Viewport.hpp"
#include "Shader.hpp"


/**
 * @brief Represents a graphics pipeline used for rendering.
 * 
 * The Pipeline class inherits from the Viewport class and encapsulates the necessary
 * functionality for creating and managing a Vulkan graphics pipeline. It provides
 * methods for setting up the pipeline layout, creating the pipeline, and accessing
 * the viewport and scissor settings.
 */
class Pipeline : public Viewport {
    const GraphicsContext& graphics_context;
    Swapchain& swapchain;

    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;


    const ShaderCollection& shaders;

public:

    Pipeline(const GraphicsContext& graphics_context, 
        Swapchain& swapchain, 
        ShaderCollection& shaders,
        bool blend = false);

    ~Pipeline();

    inline operator VkPipeline&() { return pipeline; }

    inline VkViewport& getViewport() { return viewport; }

    inline VkRect2D& getScissor() { return scissor; }

private:

    void createPipelineLayout();


};

#endif