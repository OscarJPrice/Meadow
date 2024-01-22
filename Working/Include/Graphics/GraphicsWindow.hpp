#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GraphicsWindow {
       
    GLFWwindow* window;
    VkInstance instance;

#ifndef NDEBUG
    const std::vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};
#endif

public:
    GraphicsWindow(int width, int height, const char* title);
    ~GraphicsWindow();
private:
    void initWindow(int width, int height, const char* title);
    void createInstance(const char* title);
    bool verifyExtensions(const char* const * extensions, uint32_t num_extensions);
    
    // Debugging
#ifndef NDEBUG
    bool verifyLayers();
 #endif

    void initVk(const char* title);

    void loop( );
};

#endif // GRAPHICS_WINDOW_H
