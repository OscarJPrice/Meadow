cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall)


find_package(Vulkan REQUIRED)
find_program(GLSLC_EXECUTABLE NAMES glslc)


set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/Working/Shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/SPIR-V")
file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${SHADER_SOURCE_DIR}/*.frag"
  "${SHADER_SOURCE_DIR}/*.vert"
  "${SHADER_SOURCE_DIR}/*.comp"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL}
    IMPLICIT_DEPENDS CXX ${GLSL}
    COMMENT "Compiling ${GLSL} -> ${SPIRV}"
  )
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(Shaders DEPENDS ${SPIRV_BINARY_FILES})

include_directories(./Working/Source)
include_directories(./Working/Source/Graphics)
include_directories("./Working/Source/Graphics/Environment")
include_directories("./Working/Source/Graphics/Pipeline")
include_directories(./Working/Source/Logging)

if(WIN32)
	include_directories(C:/glfw-3.3.9.bin.WIN64/include C:/VulkanSDK/1.3.275.0/Include)
	link_libraries(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/glfw3.lib )
else()
	find_package(glfw3 REQUIRED)
	link_libraries(glfw)
	add_compile_options(-Wpedantic  -Werror  -O2)
endif()

if(MSVC)
  add_compile_options(/wd4820) # Disable warning C4820: 'X': 'X' : '4' bytes padding added after data member 'Y'
	add_compile_options(/wd4668) # Disable warning C4668: 'WINVER' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	add_compile_options(/wd5045) # Disable warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
	add_compile_options(/wd4626) # Disable warning C4626: 'X': assignment operator was implicitly defined as deleted
	add_compile_options(/wd4514) # Disable warning C4514: 'X': unreferenced inline function has been removed
	add_compile_options(/wd4100) # Disable warning C4100: 'X': unreferenced formal parameter
endif()

aux_source_directory(./Working/Source SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics/Environment SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics/Pipeline SOURCE_FILES)
aux_source_directory(./Working/Source/Logging SOURCE_FILES)


add_executable(Meadow ${SOURCE_FILES})
add_dependencies(Meadow Shaders)

target_link_libraries(Meadow ${Vulkan_LIBRARIES})

