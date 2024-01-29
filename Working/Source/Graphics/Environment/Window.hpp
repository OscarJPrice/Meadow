#ifndef MEADOW_WINDOW_HPP
#define MEADOW_WINDOW_HPP

#include <GLFW/glfw3.h>

/**
 * @brief Wrapper class for the GLFW window
 * 
 */
class Window {
    GLFWwindow* window;

public:
    Window(uint32_t width, uint32_t height, const char* name);

    ~Window();

    inline operator GLFWwindow*() {
        return window;
    }

    inline operator bool() {
        return !glfwWindowShouldClose(window);
    }

};

#endif // !MEADOW_WINDOW_HPP