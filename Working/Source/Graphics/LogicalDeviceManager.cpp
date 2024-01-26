#include <set>
#include "Graphics/LogicalDeviceManager.hpp"
#include "Graphics/QueueUtils.hpp"
#include "Graphics/Instance.hpp"
#include "Graphics/Constants.hpp"
#include "ansi.h"

LogicalDeviceManager::LogicalDeviceManager(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
	
	QueueUtils::QueueFamilyIndices indices = QueueUtils::findQueueFamilies(physical_device, surface);

	if (!indices.isComplete()) {
		throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "LogicalDeviceManager.cpp " ANSI_NORMAL "failed to find queue families!");
	}

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = {
		indices.graphics_family.value(),
		indices.present_family.value()
	};

	float queue_priority = 1.0f;

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

	// Compatibility with older versions of Vulkan
	if (DEBUG_MODE) {
		create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else {
		create_info.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error(RED_FG_BRIGHT "[ERROR] " WHITE_FG_BRIGHT "LogicalDeviceManager.cpp " ANSI_NORMAL "failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &present_queue);
}

LogicalDeviceManager::~LogicalDeviceManager() {
	vkDestroyDevice(device, nullptr);
}