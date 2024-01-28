#include "Shader.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

Shader::Shader(const char* filename, const VkDevice& device) : device(device) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }
    std::vector<char> code {std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>() };

    VkShaderModuleCreateInfo shader_create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };
    if (vkCreateShaderModule(device, &shader_create_info, nullptr, &shaderModule)) {
        throw std::runtime_error("Failed to create shader module");
    }
}

Shader::~Shader() {
    vkDestroyShaderModule(device, shaderModule, nullptr);
}