#include "Graphics/Environment/QueueUtils.hpp"
#include <vector>


/**
 * @brief function to find the queue families of a physical device.
 * 
 * @param device The Vulkan physical device.
 * @param surface The Vulkan surface.
 * 
 */
QueueUtils::QueueFamilyIndices QueueUtils::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	
		QueueFamilyIndices indices{}; // Create an instance of QueueFamilyIndices struct

		{
			uint32_t queue_family_count = 0; // Variable to store the number of queue families
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr); // Get the number of queue families supported by the physical device
			std::vector<VkQueueFamilyProperties> queue_families(queue_family_count); // Create a vector to store the properties of each queue family
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data()); // Get the properties of each queue family

			int i = 0; // Counter for iterating through the queue families
			for (const auto& queue_family : queue_families) {
				VkBool32 present_support = false; // Variable to store whether the queue family supports presentation to the surface
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support); // Check if the queue family supports presentation to the surface

				if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) { // Check if the queue family supports graphics operations
					indices.graphics_family = i; // Set the graphics family index in the QueueFamilyIndices struct
				}

				if (present_support) { // Check if the queue family supports presentation
					indices.present_family = i; // Set the present family index in the QueueFamilyIndices struct
				}

				i++; // Increment the counter
			}
		}

		return indices; // Return the QueueFamilyIndices struct
	}
