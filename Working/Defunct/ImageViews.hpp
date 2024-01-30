#ifndef IMAGEVIEW_HPP
#define IMAGEVIEW_HPP

#include <vulkan/vulkan.h>
#include "SwapChain.hpp"


/**
 * @brief This class is responsible for creating image views for the swap chain images.
 * 
 * Image views are used to specify how to access the images and which part of the images to access.
 * 
 */
class ImageViews {

    std::vector<VkImageView> swap_chain_image_views;

    const VkDevice& device;
    //Swapchain& swap_chain;

public:

    ImageViews(const VkDevice& device, const Swapchain& swap_chain);    

    ~ImageViews();

    inline operator std::vector<VkImageView>&() { return swap_chain_image_views; }

};

#endif // IMAGEVIEW_HPP
