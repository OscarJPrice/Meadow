#include <iostream>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "RenderPass.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Config.h"

int main(){
	
	GraphicsContext gc("Meadow");
	Swapchain sc(gc);
	RenderPass rp (gc.getLogicalDevice(), sc.getFormat());
	sc.setRenderPass(rp);
	sc.recreate();
	
	Pipeline p(gc, sc, {
		Shader::create(SHADER_BINARY_DIR "Shader.vert.spv", gc.getLogicalDevice(), VK_SHADER_STAGE_VERTEX_BIT),
		Shader::create(SHADER_BINARY_DIR "Shader.frag.spv", gc.getLogicalDevice(), VK_SHADER_STAGE_FRAGMENT_BIT)
	}, false);

	while (gc) {

	}

}
