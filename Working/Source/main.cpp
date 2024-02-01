#include <iostream>
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "RenderPass.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "CommandPool.hpp"
#include "Frames.hpp"
#include "Config.h"
#include "collection.hpp"

int main() {
	GraphicsContext gc("Meadow");
	Swapchain sc(gc);
	RenderPass rp (gc.getLogicalDevice(), sc.getFormat());
	sc.setRenderPass(rp);

	ShaderCollection shaders (2);
	shaders[0] = Shader::create(SHADER_BINARY_DIR "Shader.vert.spv", gc.getLogicalDevice(), VK_SHADER_STAGE_VERTEX_BIT);
	shaders[1] = Shader::create(SHADER_BINARY_DIR "Shader.frag.spv", gc.getLogicalDevice(), VK_SHADER_STAGE_FRAGMENT_BIT);

	Pipeline p(gc, sc, shaders, false);
	Frames fif(gc, sc, p);

	while (gc) {
		fif.drawFrame();
	}

}
