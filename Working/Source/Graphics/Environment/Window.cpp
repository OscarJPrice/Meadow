#include "Window.hpp"

/**
 * @brief Constructs a Window object with the specified width, height, and name.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 * @param name The name of the window.
 * 
 * Vulkan needs a window to actually use, and we're using GLFW to create one.
 * Vuklan is capable of creating its own windows, but nowhere near as easily as GLFW.
 * This is like four lines of code.
 */
Window::Window(uint32_t width, uint32_t height, const char* name) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow((int)width, (int)height, name, nullptr, nullptr);
}

/**
 * @brief Destroys the Window object.
 * This is rather self explanatory, but this destructor destroys the GLFW window,
 * and then kills GLFW. This destructor is practically the last thing to be called
 * before program termination.
 */
Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}