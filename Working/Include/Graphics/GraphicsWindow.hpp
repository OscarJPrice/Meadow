#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class GraphicsWindow  {
    GLFWwindow* window;
    VkInstance instance;
    const char* name;
    int width, height;

    const std::vector<const char*> validationLayers {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifndef NDEBUG
        const bool enableValidationLayers = true;
    #else
        const bool enableValidationLayers = false;
    #endif

public:
    GraphicsWindow(int width, int height, const char* name);
    ~GraphicsWindow();
    void run();
private:
    void CreateVulkanInstance();
    void InitializeWindow();
    void InitializeGraphicsENV();

    bool CheckValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    bool CheckExtensionsSupport(std::vector<const char*> extensions);
};