#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(const VkDevice& device,
    const SwapChain& swap_chain,     
    const std::vector<VkImageView>& swap_chain_image_views, 
    const VkRenderPass& render_pass) :
    device(device)
{
    frame_buffers.resize(swap_chain_image_views.size());

    for (int i = 0; i < swap_chain_image_views.size(); i++) {


        VkFramebufferCreateInfo framebuffer_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = &swap_chain_image_views[i],
            .width = swap_chain.extent().width,
            .height = swap_chain.extent().height,
            .layers = 1
        };
        if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &frame_buffers[i])) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

FrameBuffer::~FrameBuffer() {
    for (auto framebuffer : frame_buffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}