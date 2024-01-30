#ifndef _MEADOW_GRAPHICS_CONTEXT_H_
#define _MEADOW_GRAPHICS_CONTEXT_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Instance.hpp"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> present_modes;
};

class GraphicsContext : public Window, public Instance
{
	VkSurfaceKHR surface;
	VkPhysicalDevice physical_device;

	VkDevice logical_device;
	VkQueue present_queue;

public:
	GraphicsContext(const char* name);
	~GraphicsContext();

private:
//Create functions
	void createSurface();

	void createLogicalDevice();

	void createPhysicalDevice();

//Helper functions
	static bool isPhysicalDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	static uint32_t ratePhysicalDeviceSuitability(const VkPhysicalDevice& device);

	static bool checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice& device);

public:
	static SwapChainSupportDetails queryPhysicalSwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
private:

};


#endif // _MEADOW_GRAPHICS_CONTEXT_H_