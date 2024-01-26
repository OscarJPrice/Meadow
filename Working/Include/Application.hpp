#ifndef MEADOW_APPLICATION_INCLUDE_H
#define MEADOW_APPLICATION_INCLUDE_H

#include "Graphics/Environment.hpp"

/**
 * @brief Main application class
 * 
 */
class Application {
    Window window;
    Instance instance;
    Surface surface;
    PhysicalDeviceManager physical_device;
    LogicalDeviceManager logical_device;
    SwapChain swap_chain;
    ImageView image_view;

public:
    Application(uint32_t width, uint32_t height, const char* name);

    ~Application() = default;

    void run();
};


#endif // !MEADOW_APPLICATION_INCLUDE_H
