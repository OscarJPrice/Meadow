cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wpedantic -Werror -O2)


find_package(Vulkan REQUIRED)

include_directories(./Working/Include)
include_directories(./Working/Include/Graphics)
include_directories(./Working/Include/Logging)

if(WIN32)
	include_directories(C:/glfw-3.3.9.bin.WIN64/include C:/VulkanSDK/1.3.275.0/Include)
	link_libraries(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/glfw3.lib )
elseif(APPLE)
	find_package(glfw3 REQUIRED)
	link_libraries(glfw)
endif()

if(MSVC)
    add_compile_options(/wd4820) # Disable warning C4820: 'X': 'X' : '4' bytes padding added after data member 'Y'
	add_compile_options(/wd4668) # Disable warning C4668: 'WINVER' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	add_compile_options(/wd5045) # Disable warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
endif()

aux_source_directory(./Working/Source SOURCE_FILES)
aux_source_directory(./Working/Source/Graphics SOURCE_FILES)
aux_source_directory(./Working/Source/Logging SOURCE_FILES)
add_executable(Meadow ${SOURCE_FILES})

target_link_libraries(Meadow ${Vulkan_LIBRARIES})

