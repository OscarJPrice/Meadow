#pragma once
#include <array>
#include <vulkan/vulkan.h>
#include "ModernTypes.h"



namespace CONSTANTS {
    constexpr const char* const APP_NAME = "Meadow!";
    constexpr const u32 APP_VERSION = VK_MAKE_API_VERSION(0, 1, 0, 0);
    constexpr const char* const ENGINE_NAME = "EMEAD Engine";
    constexpr const u32 ENGINE_VERSION = VK_MAKE_API_VERSION(0, 1, 0, 0);
    constexpr const u32 API_VERSION = VK_MAKE_API_VERSION(0, 1, 3, 275);

    constexpr const std::array<VkDynamicState, 3> DYNAMIC_STATES {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
    const u32 FRAMES_IN_FLIGHT = 2;

    constexpr const std::array<const char*, 3> DEVICE_EXTENSIONS {
        #ifdef __APPLE__
            "VK_KHR_portability_subset",
        #endif
            "VK_KHR_dynamic_rendering",
            "VK_KHR_swapchain"
    };

    #ifdef NDEBUG
        const bool DEBUG_MODE = false;
        const std::array<const char*, 0> VALIDATION_LAYERS {};

    #else
        const bool DEBUG_MODE = true;
        const std::array<const char*, 1> VALIDATION_LAYERS {
            "VK_LAYER_KHRONOS_validation",
        };
    #endif


    #ifdef __APPLE__
        const VkInstanceCreateFlagBits INSTANCE_FLAGS = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #else
        const VkInstanceCreateFlagBits INSTANCE_FLAGS = 0;
    #endif

}

#define SHADER_BINARY_DIR "/Users/oscarprice/code/own_p/Meadow/SPIR-V/"
#define LOG_DIR "/Users/oscarprice/code/own_p/Meadow/logs/"
