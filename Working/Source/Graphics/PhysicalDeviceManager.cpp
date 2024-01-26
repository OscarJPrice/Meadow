#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include "Graphics/PhysicalDeviceManager.hpp"
#include "Graphics/QueueUtils.hpp"
#include "ansi.h"
#include "Graphics/Constants.hpp"
#include "Logging/Logging.hpp"

PhysicalDeviceManager::PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface) {
	uint32_t count;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);

	if (count == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());
	
	uint32_t highest_score = 0;
	VkPhysicalDevice best_device = VK_NULL_HANDLE;

	for (const auto& device : devices) {
		uint32_t score = rateDeviceSuitability(device);
		if (score > highest_score) {
			if (isDeviceSuitable(device, surface)) {
				best_device = device;
				highest_score = score;
			}
			else if (best_device == VK_NULL_HANDLE || !isDeviceSuitable(best_device, surface)) {
				best_device = device;
				highest_score = score;
			}
		}
	}

	if (best_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	device = best_device;
}

/**
 * Checks if a given Vulkan physical device is suitable for rendering graphics on the graphics window.
 * 
 * @param device The Vulkan physical device to check.
 * @return True if the device is suitable, false otherwise.
 */
bool PhysicalDeviceManager::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(device, surface);
	bool extensions_supported = checkDeviceExtensionSupport(device);

	bool swap_chain_adequate = false;
	if (extensions_supported) {
		SwapChainSupportDetails swap_chain_support = querySwapChainSupport(device, surface);
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	}
	return indices.isComplete() && swap_chain_adequate && extensions_supported;
}

uint32_t PhysicalDeviceManager::rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures device_features;
	
	vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);

	//Create a score based on the device's properties.
	uint32_t score = 0;

	//Discrete GPUs have a significant performance advantage
	if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	//Maximum possible size of textures affects graphics quality
	score += device_properties.limits.maxImageDimension2D;

	return score;
}

bool PhysicalDeviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	std::set<const char*> required_extensions(device_extensions.begin(), device_extensions.end());

	for (const char* extension : device_extensions) {
		for (const auto& available_extension : available_extensions) {
			if (strcmp(extension, available_extension.extensionName) == 0) {
				goto next;
			}
		}
		#ifndef NDEBUG
			Log::error << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "Extension " << extension << " not supported!" << std::endl;
			throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "required extension not supported!");
		#endif
		return false;
		next: continue;

	}

	return true;

} 

SwapChainSupportDetails PhysicalDeviceManager::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

	if (present_mode_count != 0) {
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
	}

    return details;
}