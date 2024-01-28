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
    FrameBuffer frame_buffer;
    CommandPool command_pool;
    CommandBuffer command_buffer;

    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence unique_frame_rendered_fence;

public:
    Application(uint32_t width, uint32_t height, const char* name);

    ~Application();

    void create_sync_objects();

    void drawFrame();

    void run();
};