#include <iostream>
#include "GraphicsContext.hpp"
#include "SwapChain.hpp"
#include "RenderPass.hpp"

int main(){
	
	GraphicsContext gc("Meadow");
	Swapchain sc(gc);
	RenderPass rp (gc.getLogicalDevice(), sc.getFormat());
	while (gc) {

	}

}
