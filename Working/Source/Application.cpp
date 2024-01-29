#include "Application.hpp"
#include "CommandBuffers.hpp"
#include <stdexcept>

Application::Application(uint32_t width, uint32_t height, const char* name) :
    window(width, height, name), 
    instance(name),
    surface(instance, window),
    physical_device(instance, surface), 
    logical_device(physical_device, surface), 
    swap_chain(window, physical_device, logical_device, surface), 
    image_view(logical_device, swap_chain), 
    pipeline(logical_device, swap_chain), 
    frame_buffer(logical_device, swap_chain, image_view, pipeline), 
    command_pool(surface, physical_device, logical_device),
    command_buffers(logical_device, swap_chain, pipeline, frame_buffer, command_pool)
{
    run();
}

Application::~Application() {
}

void Application::run(){
    while(window){
        glfwPollEvents();
        command_buffers.drawFrame();
    }
    vkDeviceWaitIdle(logical_device);
}