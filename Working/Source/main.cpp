#include <iostream>
#include "Geometry.hpp"
#include "Graphics.hpp"

void loopLogic(){
	//std::cout << "Loop Logic" << std::endl;
}
int main(){
	try {
		GraphicsWindow window(800, 600, "Test");
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
