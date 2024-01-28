#include "Graphics/Include.hpp"

class Application {
    Window window; 
    Instance instance;
    Surface surface;
    PhysicalDeviceManager physical_device;
    LogicalDeviceManager logical_device;
    SwapChain swap_chain;
    ImageView image_view;
    Pipeline pipeline;

public:
    Application(uint32_t width, uint32_t height, const char* name);

    ~Application() = default;

    void run();
};