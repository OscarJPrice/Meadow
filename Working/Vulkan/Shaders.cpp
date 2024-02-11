
#include "Shaders.hpp"
#include <fstream>
#include "Debug-Macros.hpp"
#include "iostream"

ShaderModule::ShaderModule(const char* filename, const VkShaderStageFlagBits& stage, 
    const VulkanDevice* device) :
    device(device), name(filename), stage(stage)
{
    std::ifstream file(filename, std::ios::binary);
    ATTEMPT_BOOL(file.is_open());
    std::vector<char> code {std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>() };

    VkShaderModuleCreateInfo shader_create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };
    ATTEMPT_VK(vkCreateShaderModule(device->logical_device, &shader_create_info, 
        nullptr, &shader));
}

ShaderModule::ShaderModule(const ShaderModule& other) :
    device(other.device), name(other.name), shader(other.shader), stage(other.stage)
{
    //Unsafe, but let's move on to another subject!
    const_cast<VkShaderModule&>(other.shader) = VK_NULL_HANDLE;
}

ShaderModule::~ShaderModule() {
    if (shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device->logical_device, shader, nullptr);
}

std::unordered_map<std::string, ShaderModule> Shaders::all;