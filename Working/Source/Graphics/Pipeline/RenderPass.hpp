#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

#include <vulkan/vulkan.h>

class RenderPass {
    VkRenderPass render_pass;

    VkDevice& device;
public:
    RenderPass(VkDevice& device);
    ~RenderPass();

    inline operator VkRenderPass&() { return render_pass; }
};

#endif // RENDERPASS_HPP
