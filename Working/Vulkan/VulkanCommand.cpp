#include "VulkanCommand.hpp"
#include "Debug-Macros.hpp"
#include "Log.hpp"
#include "VulkanQueueUtils.hpp"

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandPool &command_pool,
                                         int n, VkCommandBufferLevel level)
    : command_pool(command_pool) {
  VkCommandBufferAllocateInfo command_buffer_allocate_info{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext = nullptr,
      .commandPool = command_pool.vk_command_pool,
      .level = level,
      .commandBufferCount = 1};
  ATTEMPT_VK(vkAllocateCommandBuffers(
      command_pool.swapchain.device.vk_logical_device,
      &command_buffer_allocate_info, &vk_command_buffer));
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
  if (vk_command_buffer != VK_NULL_HANDLE)
    vkFreeCommandBuffers(command_pool.swapchain.device.vk_logical_device,
                         command_pool.vk_command_pool, 1, &vk_command_buffer);
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer &&other) noexcept
    : command_pool(other.command_pool),
      vk_command_buffer(other.vk_command_buffer) {
  other.vk_command_buffer = VK_NULL_HANDLE;
}

void VulkanCommandBuffer::operator()(VulkanPipeline &pipeline) const {

  VkCommandBufferBeginInfo command_buffer_begin_info{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .pNext = nullptr,
      .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
      .pInheritanceInfo = nullptr};

  ATTEMPT_VK(
      vkBeginCommandBuffer(vk_command_buffer, &command_buffer_begin_info));

  // vkCmdBindPipeline(vk_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  //     pipeline.vk_pipeline);

  // vkCmdSetViewport(vk_command_buffer, 0, 1, &pipeline.viewport);

  // vkCmdSetScissor(vk_command_buffer, 0, 1, &pipeline.scissor);

  VkRenderingAttachmentInfo color_attachment_info{
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .pNext = nullptr,
      .imageView = command_pool.swapchain.image_views.at(0),
      .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = {.color = {{0.0f, 0.0f, 0.0f, 1.0f}}}};

  {
    std::cout << "color_attachment_info:\n"
              << "    .sType = " << color_attachment_info.sType << "\n"
              << "    .pNext = " << color_attachment_info.pNext << "\n"
              << "    .imageView = " << color_attachment_info.imageView << "\n"
              << "    .imageLayout = " << color_attachment_info.imageLayout
              << "\n"
              << "    .loadOp = " << color_attachment_info.loadOp << "\n"
              << "    .storeOp = " << color_attachment_info.storeOp << "\n"
              << "    .clearValue:\n"
              << "        .color:\n"
              << "            .float32[0] = "
              << color_attachment_info.clearValue.color.float32[0] << "\n"
              << "            .float32[1] = "
              << color_attachment_info.clearValue.color.float32[1] << "\n"
              << "            .float32[2] = "
              << color_attachment_info.clearValue.color.float32[2] << "\n"
              << "            .float32[3] = "
              << color_attachment_info.clearValue.color.float32[3] << "\n";
  }

  VkRenderingInfo rendering_info{
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .renderArea = {.offset = {0, 0}, .extent = command_pool.swapchain.extent},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &color_attachment_info};

  {
    std::cout << "rendering_info:\n"
              << "    .sType = " << rendering_info.sType << "\n"
              << "    .pNext = " << rendering_info.pNext << "\n"
              << "    .flags = " << rendering_info.flags << "\n"
              << "    .renderArea:\n"
              << "        .offset:\n"
              << "            .x = " << rendering_info.renderArea.offset.x
              << "\n"
              << "            .y = " << rendering_info.renderArea.offset.y
              << "\n"
              << "        .extent:\n"
              << "            .width = "
              << rendering_info.renderArea.extent.width << "\n"
              << "            .height = "
              << rendering_info.renderArea.extent.height << "\n"
              << "    .layerCount = " << rendering_info.layerCount << "\n"
              << "    .colorAttachmentCount = "
              << rendering_info.colorAttachmentCount << "\n"
              << "    .pColorAttachments = " << rendering_info.pColorAttachments
              << "\n"
              << std::endl;
  }

  Log::flush();
  vkCmdBeginRendering(vk_command_buffer, &rendering_info);

  vkCmdEndRendering(vk_command_buffer);

  ATTEMPT_VK(vkEndCommandBuffer(vk_command_buffer));
}

VulkanCommandPool::VulkanCommandPool(VulkanSwapchain &swapchain)
    : swapchain(swapchain) {
  auto queueFamilyIndices = QueueUtils::findQueueFamilies(
      swapchain.device.vk_physical_device, swapchain.device.surface.vk_surface);

  VkCommandPoolCreateInfo command_pool_create_info{
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .queueFamilyIndex = queueFamilyIndices.graphics_family.value()};
  ATTEMPT_VK(vkCreateCommandPool(swapchain.device.vk_logical_device,
                                 &command_pool_create_info, nullptr,
                                 &vk_command_pool));
}

VulkanCommandPool::~VulkanCommandPool() {
  if (vk_command_pool != VK_NULL_HANDLE) {
    command_buffers.clear();
    vkDestroyCommandPool(swapchain.device.vk_logical_device, vk_command_pool,
                         nullptr);
  }
}
