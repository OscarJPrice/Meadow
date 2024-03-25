cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Meadow VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(Meadow Source/main.cpp)

# Configure a header file to pass the version number to the source code
configure_file(MeadowConfig.h.in Source/MeadowConfig.h)

# Add the binary tree to the search path for include files
# so that we will find MeadowConfig.h
target_include_directories(Meadow PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
