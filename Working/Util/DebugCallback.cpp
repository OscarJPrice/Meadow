#include "DebugCallback.hpp"
#include "Log.hpp"
#include "ModernTypes.h"
#include "ansi.h"
#include <iostream>

static void post(std::ostream &stream, const char *message,
                 VkDebugUtilsMessageTypeFlagsEXT message_type,
                 const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
                 void *p_user_data) {
  stream << message;
  switch (message_type) {
  case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
    stream << "[GENERAL]: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
    stream << "[VALIDATION]: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
    stream << "[PERFORMANCE]: ";
    break;
  }
  stream << p_callback_data->pMessage << '\n';
  for (u32 i = 0; i < p_callback_data->objectCount; i++) {
    if (p_callback_data->pObjects[i].pObjectName) {
      stream << p_callback_data->pObjects[i].pObjectName << '\n';
    }
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback::callable(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
    void *p_user_data) {
  // Handle the debug message severity
  VkBool32 result = VK_FALSE;
  switch (message_severity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    post(Log::verbose, FAINT "[VERBOSE] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    return VK_FALSE;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    post(Log::info, GREEN_FG "[INFO] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    return VK_FALSE;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    post(std::cout, YELLOW_FG "[WARNING] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    post(Log::warning, YELLOW_FG "[WARNING] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    return VK_FALSE;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    post(std::cout, RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    post(Log::error, RED_FG_BRIGHT "[ERROR] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    return VK_TRUE;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
    post(std::cout, MAGENTA_FG_BRIGHT "[UNKNOWN] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    post(Log::info, MAGENTA_FG_BRIGHT "[UNKNOWN] " ANSI_NORMAL, message_type,
         p_callback_data, p_user_data);
    return VK_FALSE;
  }
  return result;
}

VkResult DebugCallback::create(const VkInstance &instance,
                               const VkAllocationCallbacks *allocator,
                               VkDebugUtilsMessengerEXT &debug_messenger) {
  Log::init();
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4191)
#endif

  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");

#ifdef _MSC_VER
#pragma warning(pop)
#endif

  if (func) {
    // Call the function to create the debug utils messenger extension
    return func(instance, &create_info, allocator, &debug_messenger);
  } else {
    // Return an error if the extension is not present
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VkResult DebugCallback::destroy(const VkInstance &instance,
                                const VkAllocationCallbacks *allocator,
                                VkDebugUtilsMessengerEXT &debug_messenger) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func) {
    // Call the function to destroy the debug utils messenger extension
    func(instance, debug_messenger, allocator);
    return VK_SUCCESS;
  } else {
    // Return an error if the extension is not present
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}
