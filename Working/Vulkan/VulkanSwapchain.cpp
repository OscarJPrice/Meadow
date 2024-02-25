#include "VulkanSwapchain.hpp"
#include "Debug-Macros.hpp"
#include "ModernTypes.h"
#include "VulkanQueueUtils.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapchainSupportDetails.h"
#include "vulkan/vulkan_core.h"
#include <algorithm>

namespace SwapUtils {
VkExtent2D chooseExtent(GLFWwindow *window,
                        const VkSurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
    return capabilities
        .currentExtent; // The window manager has set the extent for us
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  VkExtent2D extent{static_cast<u32>(width), static_cast<u32>(height)};

  extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                            capabilities.maxImageExtent.width);
  extent.height = std::clamp(extent.height, capabilities.minImageExtent.height,
                             capabilities.maxImageExtent.height);
  return extent;
}

VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &modes) {
  auto it = std::find(modes.begin(), modes.end(), VK_PRESENT_MODE_MAILBOX_KHR);
  return (it == modes.end()) ? VK_PRESENT_MODE_FIFO_KHR
                             : VK_PRESENT_MODE_MAILBOX_KHR;
}

std::vector<VkImageView> createImageViews(const VulkanDevice &device,
                                          const std::vector<VkImage> &images,
                                          const VkSurfaceFormatKHR &format) {
  std::vector<VkImageView> image_views;
  for (const auto &image : images) {
    VkImageViewCreateInfo image_view_create_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format.format,
        .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                       .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                       .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                       .a = VK_COMPONENT_SWIZZLE_IDENTITY},
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .baseMipLevel = 0,
                             .levelCount = 1,
                             .baseArrayLayer = 0,
                             .layerCount = 1}};
    ATTEMPT_VK(vkCreateImageView(device.vk_logical_device,
                                 &image_view_create_info, nullptr,
                                 &image_views.emplace_back()));
  }
  return image_views;
}

u32 getImageCount(VkSurfaceCapabilitiesKHR capabilities) {
  u32 image_count = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      image_count > capabilities.maxImageCount)
    image_count = capabilities.maxImageCount;
  return image_count;
}
} // namespace SwapUtils

VulkanSwapchain::VulkanSwapchain(const VulkanDevice &device,
                                 VulkanRenderPass &render_pass)
    : device(device), render_pass(render_pass) {
  create();
}

VulkanSwapchain::~VulkanSwapchain() {
  if (vk_swapchain != nullptr)
    destroy();
}

void VulkanSwapchain::create() {

  u32 image_count =
      SwapUtils::getImageCount(device.querySwapchainSupport().capabilities);

  extent = SwapUtils::chooseExtent(device.surface.window,
                                   device.querySwapchainSupport().capabilities);

  VkSwapchainCreateInfoKHR create_info = {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = device.surface.vk_surface,
      .minImageCount = image_count,
      .imageFormat = device.chooseSurfaceFormat().format,
      .imageColorSpace = device.chooseSurfaceFormat().colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};

  auto indices = QueueUtils::findQueueFamilies(device.vk_physical_device,
                                               device.surface.vk_surface);

  u32 queue_family_indices[] = {indices.graphics_family.value(),
                                indices.present_family.value()};

  ATTEMPT_BOOL(indices.isComplete());

  if (indices.graphics_family.value() != indices.present_family.value()) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  create_info.preTransform =
      device.querySwapchainSupport().capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = SwapUtils::choosePresentMode(
      device.querySwapchainSupport().present_modes);
  create_info.clipped = VK_TRUE;

  ATTEMPT_VK(vkCreateSwapchainKHR(device.vk_logical_device, &create_info,
                                  nullptr, &vk_swapchain));

  vkGetSwapchainImagesKHR(device.vk_logical_device, vk_swapchain, &image_count,
                          nullptr);
  images.resize(image_count);
  vkGetSwapchainImagesKHR(device.vk_logical_device, vk_swapchain, &image_count,
                          images.data());

  image_views =
      SwapUtils::createImageViews(device, images, device.chooseSurfaceFormat());
}

void VulkanSwapchain::destroy() {
  for (const auto &view : image_views) {
    vkDestroyImageView(device.vk_logical_device, view, nullptr);
  }
  if (frame_buffers.has_value()) {
    for (const auto &buffer : frame_buffers.value()) {
      vkDestroyFramebuffer(device.vk_logical_device, buffer, nullptr);
    }
  }
  vkDestroySwapchainKHR(device.vk_logical_device, vk_swapchain, nullptr);
}

std::vector<VkFramebuffer>
VulkanSwapchain::createFrameBuffers(const VulkanDevice &device,
                                    VulkanSwapchain &swapchain,
                                    VulkanRenderPass &render_pass) {
  std::vector<VkFramebuffer> frame_buffer;
  for (const auto &view : swapchain.image_views) {
    VkFramebufferCreateInfo framebuffer_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = render_pass.vk_render_pass,
        .attachmentCount = 1,
        .pAttachments = &view,
        .width = swapchain.extent.width,
        .height = swapchain.extent.height,
        .layers = 1};
    ATTEMPT_VK(vkCreateFramebuffer(device.vk_logical_device, &framebuffer_info,
                                   nullptr, &frame_buffer.emplace_back()));
  }
  return frame_buffer;
}
