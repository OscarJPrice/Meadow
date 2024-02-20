#include <fstream>
#include "Shaders.hpp"
#include "Debug-Macros.hpp"
#include "iostream"
#include "ModernTypes.h"

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
        .pCode = reinterpret_cast<const u32*>(code.data())
    };
    ATTEMPT_VK(vkCreateShaderModule(device->vk_logical_device, &shader_create_info, 
        nullptr, &shader));
}

ShaderModule::ShaderModule(ShaderModule&& other) noexcept:
    device(other.device),
    name(other.name),
    shader(other.shader),
    stage(other.stage)
{
    other.shader = VK_NULL_HANDLE;
}

ShaderModule& ShaderModule::operator=(ShaderModule&& other) noexcept
{
    device = other.device;
    name = other.name;
    stage = other.stage;
    shader = other.shader;
    other.shader = VK_NULL_HANDLE;
    return *this;
}


ShaderModule::~ShaderModule() {
    if (shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device->vk_logical_device, shader, nullptr);
}

VkPipelineShaderStageCreateInfo ShaderModule::shader_stage_info(const char* name) const {
    VkPipelineShaderStageCreateInfo shader_stage_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = stage,
        .module = shader,
        .pName = name
    };
    return shader_stage_info;
}

std::unordered_map<std::string, ShaderModule> Shaders::all;