#include "Application.hpp"

Application::Application(uint32_t width, uint32_t height, const char* name) :
    window(width, height, name), 
    instance(name),
    surface(instance, window),
    physical_device(instance, surface), // physical device manager must be declared after instance
    logical_device(physical_device, surface), // logical device manager must be declared after physical device manager
    swap_chain(window, physical_device, logical_device, surface) // swap chain must be declared after window, physical device manager, logical device manager, and surface
{
    run();
}