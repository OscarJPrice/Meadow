#include <iostream>
#include "Geometry.hpp"
#include "Application.hpp"

void Application::run() {
    while (window) {
        glfwPollEvents();
    }
}

int main(){
	try {
		Application app(1000, 800, "Meadow");
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
