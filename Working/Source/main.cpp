#include <iostream>
#include "Geometry.hpp"
#include "Application.hpp"

int main(){
	try {
		Application app(1000, 800, "Meadow");
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
