#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

#include <vulkan/vulkan.h>

class RenderPass {
    VkRenderPass render_pass;

    const VkDevice& device;
public:
    RenderPass(const VkDevice& device, const VkFormat& format);
    ~RenderPass();

    inline operator VkRenderPass&() { return render_pass; }
};

#endif // RENDERPASS_HPP
