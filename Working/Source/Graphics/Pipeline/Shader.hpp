#ifndef MEADOW_SHADER_HPP
#define MEADOW_SHADER_HPP

#include <vulkan/vulkan.h>
#include <memory>
#include "collection.hpp"
/**
 * @brief Represents a shader module in Vulkan.
 */
struct Shader {
    VkShaderModule shader; /**< The Vulkan shader module. */
    VkShaderStageFlagBits stage; /**< The stage of the shader. */
    const VkDevice* device; /**< Pointer to the Vulkan device. */

    /**
     * @brief Default constructor for Shader.
     */
    Shader() = default;

    /**
     * @brief Creates a shader module from a file.
     * 
     * @param filename The name of the file containing the shader code.
     * @param device The Vulkan device.
     * @param stage The stage of the shader.
     * @return The created Shader object.
     */
    static Shader create(const char* filename, const VkDevice& device, VkShaderStageFlagBits stage);

    /**
     * @brief Destroys a shader module.
     * 
     * @param shader The shader module to destroy.
     * @param device The Vulkan device.
     */
    static void destroy(const VkShaderModule& shader, const VkDevice& device);
};

/**
 * @brief A collection of Shader objects.
 * 
 * This class represents a collection of Shader objects. It inherits from the collection class and provides
 * additional functionality specific to shaders, specifically in regards to destruction.
 */
class ShaderCollection : public collection<Shader> {
public:
    /**
     * @brief Constructs a ShaderCollection object with the specified size.
     * 
     * @param size The size of the ShaderCollection.
     */
    ShaderCollection(int size);

    /**
     * @brief Destructor for the ShaderCollection object.
     */
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