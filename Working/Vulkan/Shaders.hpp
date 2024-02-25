#pragma once
#include "VulkanDevice.hpp"
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.h>

class ShaderModule {
  const VulkanDevice *device;

public:
  const char *name;
  VkShaderModule shader;
  VkShaderStageFlagBits stage;

  ShaderModule(const char *name, const VkShaderStageFlagBits &stage,
               const VulkanDevice *device);
  ShaderModule(ShaderModule &&other) noexcept;

  ShaderModule &operator=(ShaderModule &&other) noexcept;

  ~ShaderModule();

  VkPipelineShaderStageCreateInfo shader_stage_info(const char *) const;
};

struct ShaderView {
  const ShaderModule *shader;
  inline operator const ShaderModule &() const { return *shader; }
};

class Shaders {
  const std::unordered_map<std::string, ShaderModule> all;

public:
  Shaders(const VulkanDevice &device);

  ~Shaders() = default;

  static std::unordered_map<std::string, ShaderModule>
  loadShaders(const VulkanDevice &device) {
    std::unordered_map<std::string, ShaderModule> shaders;
    ShaderModule vert(SHADER_BINARY_DIR "Shader.vert.spv",
                      VK_SHADER_STAGE_VERTEX_BIT, &device);
    ShaderModule frag(SHADER_BINARY_DIR "Shader.frag.spv",
                      VK_SHADER_STAGE_FRAGMENT_BIT, &device);

    shaders.emplace(std::make_pair("vert", std::move(vert)));
    shaders.emplace(std::make_pair("frag", std::move(frag)));
    return shaders;
  }

  inline ShaderView operator[](const std::string &name) {
    return {&all.at(name)};
  }

  template <typename... T> std::vector<ShaderView> operator[](T... shaders) {
    return std::vector<ShaderView>(operator[](shaders...));
  }
};
