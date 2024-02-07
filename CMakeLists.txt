cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall)

# Shader Section
  find_program(GLSLC_EXECUTABLE NAMES glslc REQUIRED)
  message(STATUS "GLSLC Found: ${GLSLC_EXECUTABLE}")
  set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Working/Shaders")
  set(SHADER_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SPIR-V")
  file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})
  file(GLOB SHADER_FILES "${SHADER_SOURCE_DIR}/*")
  # Iterate over each shader file
  foreach(SHADER_FILE ${SHADER_FILES})
      get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
      set(SHADER_OUTPUT "${SHADER_BINARY_DIR}/${SHADER_NAME}.spv")
      # Compile each shader
      add_custom_command(
          OUTPUT ${SHADER_OUTPUT}
          COMMAND glslc ${SHADER_FILE} -o ${SHADER_OUTPUT}
          DEPENDS ${SHADER_FILE}
      )
      # Add a target for the shader
      add_custom_target(${SHADER_NAME} ALL DEPENDS ${SHADER_OUTPUT})
  endforeach()
# End Shader Section

find_package(glfw3 REQUIRED)
link_directories(${GLFW_LIBRARIES})
message(STATUS "GLFW Found: ${GLFW_LIBRARIES}")

find_package(Vulkan REQUIRED)
message(STATUS "Vulkan Found: ${Vulkan_LIBRARIES}")

# Source File Section
  aux_source_directory(Working SOURCE_FILES)
  aux_source_directory(Working/Graphics SOURCE_FILES)
  aux_source_directory(Working/Util SOURCE_FILES)
# End Source File Section
add_executable(Meadow ${SOURCE_FILES})
target_link_libraries(Meadow ${Vulkan_LIBRARIES})
target_include_directories(Meadow PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(Meadow glfw)
target_include_directories(Meadow PUBLIC ${GLFW_INCLUDE_DIRS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Working/Config.h)
# Include Directories Section
  include_directories(Working)
  include_directories(Working/Graphics)
  include_directories(Working/Util)
# End Include Directories Section

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/logs")