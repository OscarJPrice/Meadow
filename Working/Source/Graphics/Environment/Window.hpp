#ifndef MEADOW_WINDOW_HPP
#define MEADOW_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <utility>
/**
 * @brief Wrapper class for the GLFW window
 * 
 */
class Window {

protected:
    GLFWwindow* window;

public:
    Window(uint32_t width, uint32_t height, const char* name);

    ~Window();

    inline operator bool() {
        glfwPollEvents();
        return !glfwWindowShouldClose(window);
    }

    inline std::pair<uint32_t, uint32_t> getSize() {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return {(uint32_t)width, (uint32_t)height};
	}
};

#endif // !MEADOW_WINDOW_HPP