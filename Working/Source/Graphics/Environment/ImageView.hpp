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
class ImageView {

    std::vector<VkImageView> swap_chain_image_views;

    VkDevice& device;
    //SwapChain& swap_chain;

public:

    ImageView(VkDevice& device, SwapChain& swap_chain);    

    ~ImageView();

};

#endif // IMAGEVIEW_HPP
