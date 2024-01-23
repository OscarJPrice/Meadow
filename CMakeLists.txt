cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


if(WIN32)
	find_package(Vulkan REQUIRED)

	include_directories(./Working/Include C:/glfw-3.3.9.bin.WIN64/include )
	link_libraries(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/glfw3.lib )
elseif(APPLE)
	set(Vulkan_INCLUDE_DIRS "/Users/oscarprice/VulkanSDK/1.3.275.0/macOS/include" )
	set(Vulkan_LIBRARIES "/Users/oscarprice/VulkanSDK/1.3.275.0/macOS/lib/libMoltenVK.dylib" )

	include_directories(./Working/Include)
	find_package(glfw3 REQUIRED)
	link_libraries(glfw)
else()
	find_package(Vulkan REQUIRED)

	include_directories(./Working/Include)
	find_package(glfw3 REQUIRED)
	link_libraries(glfw)
endif()

include_directories(${Vulkan_INCLUDE_DIRS})

message(STATUS "Vulkan_INCLUDE_DIRS = ${Vulkan_INCLUDE_DIRS}")

if(MSVC)
    add_compile_options(/wd4820) # Disable warning C4820: 'X': 'X' : '4' bytes padding added after data member 'Y'
	add_compile_options(/wd4668) # Disable warning C4668: 'WINVER' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	add_compile_options(/wd5045) # Disable warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
endif()

aux_source_directory(./Working/Source SOURCE_FILES)

add_executable(Meadow ${SOURCE_FILES}  "Working/Source/Graphics/GraphicsWindow.cpp")

target_link_libraries(Meadow ${Vulkan_LIBRARIES})

