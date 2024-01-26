#include <iostream>
#include "Geometry.hpp"
#include "Application.hpp"

int main(){
	try {
		Application app(800, 600, "Vulkan");
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
