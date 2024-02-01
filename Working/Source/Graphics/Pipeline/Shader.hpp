#ifndef MEADOW_SHADER_HPP
#define MEADOW_SHADER_HPP

#include <vulkan/vulkan.h>
#include <memory>
#include "collection.hpp"
struct Shader {
    VkShaderModule shader;
    VkShaderStageFlagBits stage;
    const VkDevice* device;

    Shader() = default;

    static Shader create(const char* filename, const VkDevice& device, VkShaderStageFlagBits stage);

    static void destroy(const VkShaderModule& shader, const VkDevice& device);

};

class ShaderCollection : public collection<Shader> {
public:
    ShaderCollection(int size);

    ~ShaderCollection();
};

// class Shader {
//     VkShaderModule shader;

//     VkShaderStageFlagBits stage;

//     const VkDevice& device;

// public:
//     Shader(const char* filename, const VkDevice& device, VkShaderStageFlagBits stage);
//     ~Shader();
    
//     inline const VkShaderStageFlagBits& getStage() { return stage; }

//     inline operator VkShaderModule&() { return shader; }
// };
#endif // MEADOW_SHADER_HPP