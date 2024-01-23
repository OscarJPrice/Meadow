#include <iostream>
#include "Geometry.hpp"
#include "Graphics.hpp"

int main(){
	try {
		GraphicsWindow window(800, 600, "Testing");
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
