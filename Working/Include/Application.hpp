#ifndef MEADOW_APPLICATION_INCLUDE_H
#define MEADOW_APPLICATION_INCLUDE_H

#include "Graphics/Window.hpp"
#include "Graphics/Instance.hpp"
#include "Graphics/Surface.hpp"
#include "Graphics/PhysicalDeviceManager.hpp"
#include "Graphics/LogicalDeviceManager.hpp"

class Application {
    Window window;
    Instance instance;
    Surface surface;
    PhysicalDeviceManager physical_device;
    LogicalDeviceManager logical_device;

public:
    Application(uint32_t width, uint32_t height, const char* name);

    ~Application() = default;

    void run();
};


#endif // !MEADOW_APPLICATION_INCLUDE_H
