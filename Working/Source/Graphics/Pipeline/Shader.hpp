#ifndef MEADOW_SHADER_HPP
#define MEADOW_SHADER_HPP

#include <vulkan/vulkan.h>

class Shader {
    VkShaderModule shaderModule;
    const VkDevice& device;
public:
    Shader(const char* filename, const VkDevice& device);
    ~Shader();
    
    inline operator VkShaderModule&() { return shaderModule; }
};
#endif // MEADOW_SHADER_HPP