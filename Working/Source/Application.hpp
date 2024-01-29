#include "Graphics/Include.hpp"

class Application {
    Window window; 
    Instance instance;
    Surface surface;
    PhysicalDeviceManager physical_device;
    LogicalDeviceManager logical_device;
    SwapChain swap_chain;
    ImageViews image_view;
    Pipeline pipeline;
    FrameBuffer frame_buffer;
    CommandPool command_pool;
    CommandBuffers<> command_buffers;

public:
    Application(uint32_t width, uint32_t height, const char* name);

    ~Application();

    void create_sync_objects();

    void drawFrame();

    void run();
};