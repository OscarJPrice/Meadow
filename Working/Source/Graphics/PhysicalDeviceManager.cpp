#include <vector>
#include <stdexcept>
#include <iostream>
#include "Graphics/PhysicalDeviceManager.hpp"
#include "Graphics/QueueUtils.hpp"

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
	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	// bool extensions_supported = checkDeviceExtensionSupport(device);

	// bool swap_chain_adequate = false;
	// if (extensions_supported) {
	// 	SwapChainSupportDetails swap_chain_support = querySwapChainSupport(device);
	// 	swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	// }
	return indices.isComplete();// && swap_chain_adequate;// && extensions_supported;
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