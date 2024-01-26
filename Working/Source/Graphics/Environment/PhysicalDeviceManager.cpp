#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include "Graphics/Environment/PhysicalDeviceManager.hpp"
#include "Graphics/Environment/QueueUtils.hpp"
#include "Graphics/Environment/Constants.hpp"
#include "ansi.h"
#include "Logging/Logging.hpp"

/**
 * @brief Constructs a PhysicalDeviceManager object.
 *
 * This constructor initializes a PhysicalDeviceManager object with the given Vulkan instance and surface.
 * It enumerates the available physical devices, rates their suitability, and selects the best device based on the score.
 * If no suitable device is found, it throws a runtime_error.
 *
 * @param instance The Vulkan instance.
 * @param surface The Vulkan surface.
 */
PhysicalDeviceManager::PhysicalDeviceManager(const VkInstance& instance, 
	const VkSurfaceKHR& surface) 
{
	// Enumerate the available physical devices
	uint32_t count;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);

	if (count == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());

	uint32_t highest_score = 0;
	VkPhysicalDevice best_device = VK_NULL_HANDLE;

	// Iterate through each device and rate its suitability
	for (const auto& device : devices) {
		uint32_t score = rateDeviceSuitability(device);
		if (score > highest_score) {
			// Check if the device is suitable for rendering graphics on the graphics window
			if (isDeviceSuitable(device, surface)) {
				best_device = device;
				highest_score = score;
			}
			// If the current best device is not suitable, update the best device
			else if (best_device == VK_NULL_HANDLE || !isDeviceSuitable(best_device, surface)) {
				best_device = device;
				highest_score = score;
			}
		}
	}

	// If no suitable device is found, throw an error
	if (best_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	// Set the selected device as the best device
	device = best_device;
}


/**
 * Checks if a given Vulkan physical device is suitable for rendering graphics on the graphics window.
 * 
 * @param device The Vulkan physical device to check.
 * @return True if the device is suitable, false otherwise.
 */
bool PhysicalDeviceManager::isDeviceSuitable(const VkPhysicalDevice& device, 
	const VkSurfaceKHR& surface) 
{
	// Check if the device is suitable for rendering graphics on the graphics window
	QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(device, surface);
	bool extensions_supported = checkDeviceExtensionSupport(device);

	bool swap_chain_adequate = false;
	if (extensions_supported) {
		// Query the swap chain support details
		SwapChainSupportDetails swap_chain_support = querySwapChainSupport(device, surface);
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	}
	// Return true if the device has complete queue families, adequate swap chain support, and supported extensions
	return indices.isComplete() && swap_chain_adequate && extensions_supported;
}

/**
 * Rates a given Vulkan physical device based on its properties.
 * 
 * @param device The Vulkan physical device to rate.
 * @return The score of the device.
 */
uint32_t PhysicalDeviceManager::rateDeviceSuitability(const VkPhysicalDevice& device) {
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


/**
 * Checks if a given Vulkan physical device supports the required extensions.
 * 
 * @param device The Vulkan physical device to check.
 * @return True if the device supports the required extensions, false otherwise.
 */
bool PhysicalDeviceManager::checkDeviceExtensionSupport(const VkPhysicalDevice& device) {
	// Enumerate the available device extensions
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	// Create a set of required extensions
	std::set<const char*> required_extensions(device_extensions.begin(), device_extensions.end());

	// Check if each required extension is supported by the device
	for (const char* extension : device_extensions) {
		for (const auto& available_extension : available_extensions) {
			if (strcmp(extension, available_extension.extensionName) == 0) {
				// Extension is supported, continue to the next one
				goto next;
			}
		}
		// Required extension is not supported, throw an error
		#ifndef NDEBUG
			Log::error << RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "Extension " << extension << " not supported!" << std::endl;
			throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL "required extension not supported!");
		#endif
		return false;
		next: continue;
	}

	// All required extensions are supported
	return true;

} 


/**
 * Queries the swap chain support details of a given Vulkan physical device.
 * 
 * @param device The Vulkan physical device to query.
 * @param surface The Vulkan surface.
 * @return The swap chain support details of the device.
 */
SwapChainSupportDetails PhysicalDeviceManager::querySwapChainSupport(
	const VkPhysicalDevice& device, const VkSurfaceKHR& surface) 
{

	// Query the surface capabilities of the physical device
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Query the available surface formats
	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	// Query the available presentation modes
	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

	if (present_mode_count != 0) {
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
	}

	// Return the swap chain support details
	return details;

}