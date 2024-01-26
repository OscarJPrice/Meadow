#ifndef DEBUG_CONSTANTS_HPP
#define DEBUG_CONSTANTS_HPP

#include <vector>

const static std::vector<const char*> VALIDATION_LAYERS {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> device_extensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
    "VK_KHR_portability_subset",
#endif
};

#ifdef NDEBUG
    const static bool DEBUG_MODE = false;
#else
    const static bool DEBUG_MODE = true;
#endif


#endif // DEBUG_CONSTANTS_HPP
