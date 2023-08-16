cmake_minimum_required(VERSION 3.10)

# Project setup
project(vk-spaceships LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Libs
add_subdirectory(libs/bgfx.cmake)
find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan)

# set(OpenGL_GL_PREFERENCE "GLVND")
# find_package(OpenGL)

# Sources
set(SRCS src/main.cpp src/window.cpp src/renderer.cpp src/resources.cpp)


add_executable(vk-spaceships ${SRCS})
target_link_libraries(vk-spaceships bx bgfx glfw Vulkan::Vulkan)
target_include_directories(vk-spaceships PUBLIC libs)
