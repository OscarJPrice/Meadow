#include "Graphics/Environment/ImageView.hpp"


ImageView::ImageView(SwapChain& swap_chain, VkDevice& device)
    : device(device)//, swap_chain(swap_chain)
{
    swap_chain_image_views.resize(swap_chain.images().size());
    int i = 0;
    for (const auto& image : swap_chain.images()) {
        VkImageViewCreateInfo image_view_create_info {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swap_chain,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        if (vkCreateImageView(device, &image_view_create_info, nullptr, 
            swap_chain_image_views.data() + i) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
        i++;
    }
}

ImageView::~ImageView() {
    for (auto& image_view : swap_chain_image_views) {
        vkDestroyImageView(device, image_view, nullptr);
    }
}