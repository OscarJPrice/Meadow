#include "Graphics/GraphicsWindow.hpp"
#include <cstring>
#include "ansi.h"

GraphicsWindow::GraphicsWindow(int width, int height, const char* name) : width(width), height(height), name(name) {
    InitializeWindow();
    InitializeGraphicsENV();
    run();
}

GraphicsWindow::~GraphicsWindow() {
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void GraphicsWindow::InitializeWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

void GraphicsWindow::CreateVulkanInstance() {
    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = name,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0
    };

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    std::vector<const char*> extensions = getRequiredExtensions();
    if (!CheckExtensionsSupport(extensions)) {
        throw std::runtime_error("not all extensions found!");
    }

    #ifdef __APPLE__
        //very fucking necessary
		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; 
    #endif

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
            
    auto result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void GraphicsWindow::InitializeGraphicsENV() {
    CreateVulkanInstance();
}

void GraphicsWindow::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

std::vector<const char*> GraphicsWindow::getRequiredExtensions() {
    uint32_t glfwExtensionCount;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    #ifdef __APPLE__ 
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    if (enableValidationLayers) {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool GraphicsWindow::CheckExtensionsSupport(std::vector<const char*> extensions) {
    std::vector<VkExtensionProperties> available_extensions;
    {
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        available_extensions.resize(extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
    }

    //this is used in place of std::includes because it allows for debug info.
    for (const char* extension : extensions) {
        for (const auto& available_extension : available_extensions) {
            if (strcmp(extension, available_extension.extensionName) == 0) {
                goto next;
            }
        }
        return false;
        next: continue;
    }
    return true;
} 

bool GraphicsWindow::CheckValidationLayerSupport() {
    
	std::vector<VkLayerProperties> available_layers;
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        available_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 
    }

    //this is used in place of std::includes because it allows for debug info.
    for (const char* layer : validationLayers) {
        for (VkLayerProperties available_layer : available_layers) {
            if (strcmp(layer, available_layer.layerName) == 0) {
                goto next;
            }
        }
        return false;
        next: continue;
    }
    return true;
}