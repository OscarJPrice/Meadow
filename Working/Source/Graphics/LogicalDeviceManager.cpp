#include <set>
#include "Graphics/LogicalDeviceManager.hpp"
#include "Graphics/QueueUtils.hpp"
#include "Graphics/Instance.hpp"
#include "Graphics/Constants.hpp"
#include "ansi.h"

/**
 * @brief Constructs a LogicalDeviceManager object.
 *
 * This constructor initializes a LogicalDeviceManager object with the given physical device and surface.
 * It finds the queue families supported by the physical device for graphics and presentation.
 * If the required queue families are not found, it throws a runtime_error.
 * It creates a set of unique queue families and creates a device queue for each unique queue family.
 * It also creates the logical device and gets the present queue from the logical device.
 *
 * @param physical_device The Vulkan physical device.
 * @param surface The Vulkan surface.
 */
LogicalDeviceManager::LogicalDeviceManager(VkPhysicalDevice& physical_device, VkSurfaceKHR& surface) {

	// Find the queue families supported by the physical device for graphics and presentation
	QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(physical_device, surface);

	// Throw an error if the required queue families are not found
	if (!indices.isComplete()) {
		throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT 
			"LogicalDeviceManager.cpp " ANSI_NORMAL "failed to find queue families!");
	}

	// Create a set of unique queue families
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = {
		indices.graphics_family.value(),
		indices.present_family.value()
	};

	float queue_priority = 1.0f;

	// Create a device queue for each unique queue family
	for (const auto& queue_family : unique_queue_families) {
		VkDeviceQueueCreateInfo queue_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = queue_family,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
		};
		queue_create_infos.push_back(queue_create_info);
	}

	VkPhysicalDeviceFeatures device_features{};
	VkDeviceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = queue_create_infos.data(),
		.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
		.pEnabledFeatures = &device_features,
		.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
		.ppEnabledExtensionNames = device_extensions.data(),
	};

	// Enable validation layers if in debug mode
	if (DEBUG_MODE) {
		create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else {
		create_info.enabledLayerCount = 0;
	}

	// Create the logical device
	if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "LogicalDeviceManager.cpp " ANSI_NORMAL "failed to create logical device!");
	}

	// Get the present queue from the logical device
	vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &present_queue);
}

/**
 * @brief Destroys a LogicalDeviceManager object.
 *
 * This destructor destroys the logical device.
 */
LogicalDeviceManager::~LogicalDeviceManager() {
	vkDestroyDevice(device, nullptr);
}