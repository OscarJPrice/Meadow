#pragma once
#include "VulkanDevice.hpp"
#include <map>
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

struct Shaders {

  static std::unordered_map<std::string, ShaderModule> all;

  inline static void add(const std::string &name, ShaderModule &shader) {
    all.emplace(std::make_pair(name, std::move(shader)));
  }

  inline static void loadShaders(const VulkanDevice &device) {
    ShaderModule vert(SHADER_BINARY_DIR "Shader.vert.spv",
                      VK_SHADER_STAGE_VERTEX_BIT, &device);
    ShaderModule frag(SHADER_BINARY_DIR "Shader.frag.spv",
                      VK_SHADER_STAGE_FRAGMENT_BIT, &device);
    add("vert", vert);
    add("frag", frag);
  }

  inline static void cleanup() { all.clear(); }

  inline static const ShaderModule &get(const std::string &name) {
    return all.at(name);
  }
};
