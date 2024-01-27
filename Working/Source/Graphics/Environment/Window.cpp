#include "Graphics/Environment/Window.hpp"

/**
 * @brief Constructs a Window object with the specified width, height, and name.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 * @param name The name of the window.
 */
Window::Window(uint32_t width, uint32_t height, const char* name) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow((int)width, (int)height, name, nullptr, nullptr);
}

/**
 * @brief Destroys the Window object.
 *
 */
Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}