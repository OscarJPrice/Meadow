#ifndef GRAPHICS_WINDOW_HPP
#define GRAPHICS_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

class GraphicsWindow  {
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    const char* name;

    const std::vector<const char*> validation_layers {
        "VK_LAYER_KHRONOS_validation"
    };
    #ifndef NDEBUG
        const bool ENABLE_VALIDATION_LAYERS = true;
    #else
        const bool ENABLE_VALIDATION_LAYERS = false;
    #endif


public:
    GraphicsWindow(int width, int height, const char* name);
    ~GraphicsWindow();

private:

    void run();
    void createVulkanInstance();
    void initializeWindow(int width, int height);
    void initializeGraphicsENV();
    int rateDeviceSuitability(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void setupDebugCallbackSys();
    void debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);


    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    bool checkExtensionsSupport(std::vector<const char*> extensions);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void* p_user_data
    );
};

#endif