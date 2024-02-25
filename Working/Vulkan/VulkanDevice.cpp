#include "VulkanDevice.hpp"
#include "Debug-Macros.hpp"
#include "Log.hpp"
#include "ModernTypes.h"
#include "VulkanQueueUtils.hpp"
#include "VulkanSwapchainSupportDetails.h"
#include "ansi.h"
#include <set>
#include <utility>

bool checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice &device) {
  // Enumerate the available device extensions
  u32 extension_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       available_extensions.data());

  // Create a set of required extensions
  std::set<const char *> required_extensions(
      CONSTANTS::DEVICE_EXTENSIONS.begin(), CONSTANTS::DEVICE_EXTENSIONS.end());

  // Check if each required extension is supported by the device
  for (const char *extension : CONSTANTS::DEVICE_EXTENSIONS) {
    for (const auto &available_extension : available_extensions) {
      if (strcmp(extension, available_extension.extensionName) == 0) {
        // Extension is supported, continue to the next one
        goto next;
      }
    }
    // Required extension is not supported, throw an error
#ifndef NDEBUG
    Log::error << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "Extension " << extension
               << " not supported!\n";
    throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL
                                           "required extension not supported!");
#endif
    return false;
  next:
    continue;
  }

  // All required extensions are supported
  return true;
}

SwapchainSupportDetails
VulkanDevice::querySwapchainSupport(const VkPhysicalDevice &device,
                                    const VkSurfaceKHR &surface) {

  // Query the surface capabilities of the physical device
  SwapchainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  // Query the available surface formats
  u32 format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

  if (format_count != 0) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count,
                                         details.formats.data());
  }

  // Query the available presentation modes
  u32 present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                            &present_mode_count, nullptr);

  if (present_mode_count != 0) {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &present_mode_count, details.present_modes.data());
  }

  // Return the swap chain support details
  return details;
}

bool isPhysicalDeviceSuitable(const VkPhysicalDevice &device,
                              const VkSurfaceKHR &surface) {
  // Check if the device is suitable for rendering graphics on the graphics
  // window
  QueueUtils::QueueFamilyIndices indices =
      QueueUtils::findQueueFamilies(device, surface);
  bool extensions_supported = checkPhysicalDeviceExtensionSupport(device);

  bool swap_chain_adequate = false;
  if (extensions_supported) {
    // Query the swap chain support details
    SwapchainSupportDetails swap_chain_support =
        VulkanDevice::querySwapchainSupport(device, surface);
    swap_chain_adequate = !swap_chain_support.formats.empty() &&
                          !swap_chain_support.present_modes.empty();
  }
  // Return true if the device has complete queue families, adequate swap chain
  // support, and supported extensions
  return indices.isComplete() && swap_chain_adequate && extensions_supported;
}

u32 ratePhysicalDeviceSuitability(const VkPhysicalDevice &device) {
  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;

  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);

  // Create a score based on the device's properties.
  u32 score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += device_properties.limits.maxImageDimension2D;

  return score;
}

VkPhysicalDevice aquirePhysicalDevice(VkInstance &instance,
                                      VkSurfaceKHR &surface) {
  // Enumerate the available physical devices
  u32 count;
  vkEnumeratePhysicalDevices(instance, &count, nullptr);

  if (count == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(count);
  vkEnumeratePhysicalDevices(instance, &count, devices.data());

  u32 highest_score = 0;
  VkPhysicalDevice best_device = nullptr;

  // Iterate through each device and rate its suitability
  for (const auto &phsyical_device : devices) {
    u32 score = ratePhysicalDeviceSuitability(phsyical_device);
    if (score > highest_score) {
      // Check if the device is suitable for rendering graphics on the graphics
      // window
      if (isPhysicalDeviceSuitable(phsyical_device, surface)) {
        best_device = phsyical_device;
        highest_score = score;
      }
      // If the current best device is not suitable, update the best device
      else if (best_device == nullptr ||
               !isPhysicalDeviceSuitable(best_device, surface)) {
        best_device = phsyical_device;
        highest_score = score;
      }
    }
  }

  // If no suitable device is found, throw an error
  if (best_device == nullptr) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

  // Set the selected device as the best device
  return best_device;
}

std::pair<VkDevice, VkQueue>
createLogicalDevice(VkPhysicalDevice &vk_physical_device,
                    VkSurfaceKHR &surface) {
  // Find the queue families supported by the physical device for graphics and
  // presentation
  QueueUtils::QueueFamilyIndices indices =
      QueueUtils::findQueueFamilies(vk_physical_device, surface);

  // Throw an error if the required queue families are not found
  if (!indices.isComplete()) {
    throw std::runtime_error(RED_FG_BRIGHT
                             "[ERROR] " WHITE_FG_BRIGHT
                             "LogicalDeviceManager.cpp " ANSI_NORMAL
                             "failed to find queue families!");
  }

  // Create a set of unique queue families
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<u32> unique_queue_families = {indices.graphics_family.value(),
                                         indices.present_family.value()};

  float queue_priority = 1.0f;

  // Create a device queue for each unique queue family
  for (const auto &queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queue_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority};
    queue_create_infos.push_back(queue_create_info);
  }
  VkPhysicalDeviceFeatures device_features{};
  VkDeviceCreateInfo create_info{
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = nullptr,
      .queueCreateInfoCount = static_cast<u32>(queue_create_infos.size()),
      .pQueueCreateInfos = queue_create_infos.data(),
      .enabledExtensionCount =
          static_cast<u32>(CONSTANTS::DEVICE_EXTENSIONS.size()),
      .ppEnabledExtensionNames = CONSTANTS::DEVICE_EXTENSIONS.data(),
      .pEnabledFeatures = &device_features};

  // Enable validation layers if in debug mode
  if (CONSTANTS::DEBUG_MODE) {
    create_info.enabledLayerCount =
        static_cast<u32>(CONSTANTS::VALIDATION_LAYERS.size());
    create_info.ppEnabledLayerNames = CONSTANTS::VALIDATION_LAYERS.data();
  } else {
    create_info.enabledLayerCount = 0;
  }
  VkDevice vk_logical_device;
  // Create the logical device
  ATTEMPT_VK(vkCreateDevice(vk_physical_device, &create_info, nullptr,
                            &vk_logical_device));

  // Get the present queue from the logical device
  VkQueue present_queue;
  vkGetDeviceQueue(vk_logical_device, indices.graphics_family.value(), 0,
                   &present_queue);

  return {vk_logical_device, present_queue};
}

VulkanDevice::VulkanDevice(VulkanInstance &instance, VulkanSurface &surface)
    : surface(surface), vk_physical_device(aquirePhysicalDevice(
                            instance.vk_instance, surface.vk_surface)) {
  std::tie(vk_logical_device, present_queue) =
      createLogicalDevice(vk_physical_device, surface.vk_surface);
}

VulkanDevice::~VulkanDevice() {
  if (vk_logical_device != nullptr)
    vkDestroyDevice(vk_logical_device, nullptr);
}

VkSurfaceFormatKHR VulkanDevice::chooseSurfaceFormat() const {
  auto formats =
      querySwapchainSupport(vk_physical_device, surface.vk_surface).formats;
  auto it = std::find_if(
      formats.begin(), formats.end(), [](const VkSurfaceFormatKHR &format) {
        return format.format == VK_FORMAT_B8G8R8A8_SRGB &&
               format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
      });
  return (it == formats.end()) ? formats[0] : *it;
}
