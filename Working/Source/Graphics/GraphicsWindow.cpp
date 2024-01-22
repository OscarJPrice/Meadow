#include "Graphics/GraphicsWindow.hpp"
#include <iostream>
GraphicsWindow::GraphicsWindow(int width, int height, const char* title)
{
	initWindow(width, height, title);
	initVk(title);
	loop();
}

GraphicsWindow::~GraphicsWindow()
{
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();	
}

void GraphicsWindow::initWindow(int width, int height, const char* title) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

bool GraphicsWindow::verifyExtensions(const char* const* extensions, uint32_t num_extensions) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (uint32_t i = 0; i < num_extensions; i++) { 
		bool found = false;
		for (uint32_t j = 0; j < glfwExtensionCount; j++) {
			std::cout<< extensions[i] << " " << glfwExtensions[j] << std::endl;
			if (strcmp(extensions[i], glfwExtensions[j]) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout << "Extension " << extensions[i] << " not found!" << std::endl;
			return false;
		}
	}

	return true;

}

#ifndef NDEBUG
bool GraphicsWindow::verifyLayers() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> available_layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, available_layers.data());
	std::cout<< layerCount << std::endl;
	for (const auto& layer : validation_layers) {
		bool found = false;
		for (uint32_t i = 0; i < layerCount; i++) {
			std::cout<< layer << " " << available_layers[i].layerName << std::endl;
			if (strcmp(layer, available_layers[i].layerName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout<< "Layer " << layer << " not found!" << std::endl;
			return false;
		}

	}
	return true;
}
#endif // NDEBUG

void GraphicsWindow::createInstance(const char* title) {
	// Create the instance
	VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = title,
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};
	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
	};
	
	createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);


	#ifdef __APPLE__ // Apple requires the portability subset extension to be enabled
		char** extensions = new char* [createInfo.enabledExtensionCount + 1];
		for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
			extensions[i] = new char[200];
			strncpy(extensions[i], createInfo.ppEnabledExtensionNames[i], 200);
		}
		extensions[createInfo.enabledExtensionCount] = new char[200];
		strncpy(extensions[createInfo.enabledExtensionCount], "VK_KHR_portability_subset", 200);

		createInfo.enabledExtensionCount++;
		createInfo.ppEnabledExtensionNames = (const char* const*)extensions;
	#endif
	
	#ifndef NDEBUG // Validation layers are only enabled in debug mode
		if (!verifyLayers()) {
			throw std::runtime_error("Failed to create instance, not all layers present!");
		}
		else {
			createInfo.enabledLayerCount = (uint32_t)validation_layers.size();
			createInfo.ppEnabledLayerNames = validation_layers.data();

			char** extensions = new char*[createInfo.enabledExtensionCount + 1];
			for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
				extensions[i] = new char[200];
				strncpy(extensions[i], createInfo.ppEnabledExtensionNames[i], 200);
				#ifndef __APPLE__
				delete[] createInfo.ppEnabledExtensionNames[i];
				#endif	
			}
			#ifndef __APPLE__
			delete[] createInfo.ppEnabledExtensionNames;
			#endif
			extensions[createInfo.enabledExtensionCount] = new char[200];
			strncpy(extensions[createInfo.enabledExtensionCount], "VK_EXT_debug_utils", 200);

			createInfo.enabledExtensionCount++;
			createInfo.ppEnabledExtensionNames = (const char* const*)extensions;
		}
	#endif // NDEBUG

	// Check if all extensions are present
	if (!verifyExtensions(createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount)) {
		throw std::runtime_error("Failed to create instance, not all extensions present!");
	}

	// Create the instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance!");
	}
}


void GraphicsWindow::initVk(const char* title) {
	createInstance(title);
}

void GraphicsWindow::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}