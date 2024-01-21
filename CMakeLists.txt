cmake_minimum_required(VERSION 3.5.0)
project(Meadow VERSION 0.1.0 LANGUAGES C CXX)

#include(CTest)
#enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

include_directories(./Working/Include)

aux_source_directory(./Working/Source SOURCE_FILES)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Working)
add_executable(Meadow ${SOURCE_FILES})

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#`set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
