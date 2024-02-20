#pragma once

#include <stdexcept>
#include <string>

#ifndef NDEBUG
    #define ATTEMPT_VK(vk_code) \
        {\
            int result;\
            if ((result = (vk_code))) { throw std::runtime_error(std::string( \
            "result != vkSuccess (result = " + std::to_string(result) + ") from: \
            " #vk_code " in file " __FILE__ "(line ") + std::to_string(__LINE__) + ")"); }\
        }

    #define ATTEMPT_BOOL(bool_code) \
        if (!(bool_code)) { throw std::runtime_error(std::string("Statement " \
        #bool_code " failed in file ") + __FILE__ + " (line " + \
        std::to_string(__LINE__) +")"); }
#else
    #define ATTEMPT_VK(vk_code) vk_code
    #define ATTEMPT_BOOL(bool_code) bool_code
#endif