#ifndef MEADOW_SHADER_HPP
#define MEADOW_SHADER_HPP

#include <vulkan/vulkan.h>

class Shader {
    VkShaderModule shaderModule;
    VkDevice& device;
public:
    Shader(const char* filename, VkDevice& device);
    ~Shader();
    
    inline operator VkShaderModule&() { return shaderModule; }
};
#endif // MEADOW_SHADER_HPP