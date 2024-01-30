#ifndef _MEADOW_GRAPHICS_CONTEXT_H_
#define _MEADOW_GRAPHICS_CONTEXT_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Instance.hpp"
#include "SwapchainSupportDetails.h"

class GraphicsContext : public Window, public Instance
{
	VkSurfaceKHR surface;
	VkPhysicalDevice physical_device;

	VkDevice logical_device;
	VkQueue present_queue;

public:
	GraphicsContext(const char* name);
	~GraphicsContext();

	inline const VkSurfaceKHR& getSurface() const { return surface; }

	inline const VkPhysicalDevice& getPhysicalDevice() const { return physical_device; }

	inline const VkDevice& getLogicalDevice() const { return logical_device; }

	inline const VkQueue& getPresentQueue() const { return present_queue; }

	inline GLFWwindow* getWindow() const { return window; }

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
	static SwapchainSupportDetails queryPhysicalSwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	inline SwapchainSupportDetails queryPhysicalSwapChainSupport() const { return queryPhysicalSwapChainSupport(physical_device, surface); }
private:

};


#endif // _MEADOW_GRAPHICS_CONTEXT_H_