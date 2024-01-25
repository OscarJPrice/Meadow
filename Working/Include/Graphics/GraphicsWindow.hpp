#ifndef GRAPHICS_WINDOW_HPP
#define GRAPHICS_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <optional>

/**
 * @class GraphicsWindow
 * @brief Represents a graphics window for rendering graphics using Vulkan.
 */
class GraphicsWindow  {
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphics_queue;
    VkSurfaceKHR surface;
    VkQueue present_queue;

    const std::vector<const char*> validation_layers {
        "VK_LAYER_KHRONOS_validation"
    };
#ifndef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = true;
#else
    const bool ENABLE_VALIDATION_LAYERS = false;
#endif


public:
    GraphicsWindow(uint32_t width, uint32_t height, const char* name);
    ~GraphicsWindow();

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        inline bool isComplete() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };


    void run();
    void createVulkanInstance(const char* name);
    void initializeWindow(uint32_t width, uint32_t height, const char* name);
    void initializeGraphicsENV(const char* name);
    uint32_t rateDeviceSuitability(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createSurface();
    void pickPhysicalDevice();
    void setupDebugCallbackSys();
    void debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
    void createLogicalDevice();


    bool checkValidationLayerSupport();
    std::vector<const char*> getInstanceRequiredExtensions();
    bool checkExtensionsSupport(std::vector<const char*> extensions);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void* p_user_data
    );
};

#endif