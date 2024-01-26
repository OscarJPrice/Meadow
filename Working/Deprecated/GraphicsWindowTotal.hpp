#ifndef GRAPHICS_WINDOW_HPP
#define GRAPHICS_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <optional>

/**
 * @class GraphicsWindowTotal
 * @brief Represents a graphics window for rendering graphics using Vulkan.
 */
class GraphicsWindowTotal {
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

    // Enable validation layers only in debug mode
#ifndef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = true;
#else
    const bool ENABLE_VALIDATION_LAYERS = false;
#endif

public:
    GraphicsWindowTotal(uint32_t width, uint32_t height, const char* name);
    ~GraphicsWindowTotal();

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        // Check if both graphics and present families are available
        inline bool isComplete() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    // Run the graphics window
    void run();

    // Create Vulkan instance
    void createVulkanInstance(const char* name);

    // Initialize the GLFW window
    void initializeWindow(uint32_t width, uint32_t height, const char* name);

    // Initialize the graphics environment
    void initializeGraphicsENV(const char* name);

    // Rate the suitability of a physical device
    uint32_t rateDeviceSuitability(VkPhysicalDevice device);

    // Check if a physical device is suitable for the application
    bool isDeviceSuitable(VkPhysicalDevice device);

    // Check if a physical device supports required extensions
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    // Find queue families supported by a physical device
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    // Populate swap chain details
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

    // Choose a swap chain present mode
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

    // Choose a swap chain extent
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    // Create a swap chain
    void createSwapChain();

    // Create a surface for rendering
    void createSurface();

    // Pick a suitable physical device
    void pickPhysicalDevice();

    // Setup debug callback system
    void setupDebugCallbackSys();

    // Populate debug messenger create info
    void debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

    // Create logical device
    void createLogicalDevice();

    // Check if validation layers are supported
    bool checkValidationLayerSupport();

    // Get required extensions for the Vulkan instance
    std::vector<const char*> getInstanceRequiredExtensions();

    // Check if all required extensions are supported
    bool checkExtensionsSupport(std::vector<const char*> extensions);

    // Debug callback function
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void* p_user_data
    );
};

#endif