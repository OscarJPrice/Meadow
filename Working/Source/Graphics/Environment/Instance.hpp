#ifndef MEADOW_INSTANCE_HPP
#define MEADOW_INSTANCE_HPP


#include <vulkan/vulkan.h>
#include <vector>

/**
 * @brief Wrapper class for the Vulkan Instance
 * 
 */
class Instance {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;

public:

    Instance(const char* name); //this constructor can throw exceptions

    ~Instance();

    inline operator VkInstance&() {
        return instance;
    }

private:
    std::vector<const char*> getRequiredExtensions();

    bool checkExtensionsSupport(std::vector<const char*> extensions);

    bool checkValidationLayerSupport();
    
    void setupDebugCallbackSys();

    void debugMessengerPopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

    VkResult createDebugUtilsMessengerExtension(
        const VkInstance& instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* create_info, 
        const VkAllocationCallbacks* allocator, 
        VkDebugUtilsMessengerEXT& debug_messenger_arg
    );

    VkResult destroyDebugUtilsMessengerExtension(
        const VkInstance& instance_arg, 
        const VkAllocationCallbacks* allocator,
        VkDebugUtilsMessengerEXT& debug_messenger_arg 
    );

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
        VkDebugUtilsMessageTypeFlagsEXT message_type, 
        const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
        void *p_user_data
    );
};

#endif // !INSTANCE_HPP