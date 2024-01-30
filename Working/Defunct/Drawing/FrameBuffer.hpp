#ifndef MEADOW_FRAMEBUFFER_HPP
#define MEADOW_FRAMEBUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include "SwapChain.hpp"

class FrameBuffer
{
    std::vector<VkFramebuffer> frame_buffers;
    const VkDevice& device;
public:
    FrameBuffer(const VkDevice& device, const SwapChain& swap_chain,     
        const std::vector<VkImageView>& swap_chain_image_views, 
        const VkRenderPass& render_pass);
    
    ~FrameBuffer();

    inline operator std::vector<VkFramebuffer>&() { return frame_buffers; }
    inline VkFramebuffer& operator[](uint32_t n) { return frame_buffers[n]; }


};
#endif // !MEADOW_FRAMEBUFFER_HPP