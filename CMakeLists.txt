cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall)


find_package(Vulkan REQUIRED)
find_program(GLSLC_EXECUTABLE NAMES glslc)

if(NOT GLSLC_EXECUTABLE)
  message(FATAL_ERROR "glslc executable not found")
endif()

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Working/Shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SPIR-V")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Working/Config.h)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/logs")

file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

# Find all shader files
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

include_directories(./Working/Source)
include_directories(./Working/Source/Graphics)
include_directories(./Working/Source/Graphics/Environment)
include_directories(./Working/Source/Graphics/Pipeline)
#include_directories(./Working/Source/Graphics/Drawing)
include_directories(./Working/Source/Graphics/Swapchain)
include_directories(./Working/Source/Logging)
include_directories(./Working/)

if(WIN32)
	include_directories(C:/glfw-3.3.9.bin.WIN64/include C:/VulkanSDK/1.3.275.0/Include)
	link_libraries(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/glfw3.lib )
else()
	find_package(glfw3 REQUIRED)
	link_libraries(glfw)
endif()

if(MSVC)
  add_compile_options(/wd4820) # Disable warning C4820: 'X': 'X' : '4' bytes padding added after data member 'Y'
	add_compile_options(/wd4668) # Disable warning C4668: 'WINVER' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	add_compile_options(/wd5045) # Disable warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
	add_compile_options(/wd4626) # Disable warning C4626: 'X': assignment operator was implicitly defined as deleted
	add_compile_options(/wd4514) # Disable warning C4514: 'X': unreferenced inline function has been removed
	add_compile_options(/wd4100) # Disable warning C4100: 'X': unreferenced formal parameter
	add_compile_options(/wd4458) # Disable warning C4458: declaration of 'X' hides class member
else()
  add_compile_options(-Wpedantic  -Werror  -O3)
endif()

aux_source_directory(./Working/Source SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics/Environment SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics/Pipeline SOURCE_FILES)
#aux_source_directory(./Working/Source/Graphics/Drawing SOURCE_FILES)
aux_source_directory(./Working/Source/Logging SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics/Swapchain SOURCE_FILES)


add_executable(Meadow ${SOURCE_FILES})
#add_dependencies(Meadow Shaders)

target_link_libraries(Meadow ${Vulkan_LIBRARIES})

