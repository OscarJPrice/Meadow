#ifndef COMMANDBUFFER_HPP
#define COMMANDBUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include "Pipeline.hpp"

class CommandBuffer {
    VkCommandBuffer command_buffer;
    const VkCommandPool& command_pool;
    const VkDevice& device;
    Pipeline& pipeline;
    const std::vector<VkFramebuffer>& frame_buffers;
    const VkExtent2D& swap_chain_extent;

public:
    CommandBuffer(const VkCommandPool& command_pool, const VkDevice& device, 
        Pipeline& pipeline, const std::vector<VkFramebuffer>& frame_buffers, 
        const VkExtent2D& swap_chain_extent);

    ~CommandBuffer();

    inline operator VkCommandBuffer&() { return command_buffer; }

    inline operator VkCommandBuffer*() { return &command_buffer; }

    void record(uint32_t image_index);

    void reset(uint32_t image_index = 0);

};

#endif // COMMANDBUFFER_HPP
