#include "window.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

void window::init() {
    if (!glfwInit()) {
        std::cerr << "GLFW Init Failed !\n";
        return;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "Vulkan not supported !\n";
        return;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Spaceships", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW Window creation failed !\n";
        return;
    }

    nat_win = (void*) (std::uintptr_t) glfwGetX11Window(window);
    nat_dis = glfwGetX11Display();
}

void window::setResizeCallback(GLFWframebuffersizefun resize) {
    glfwSetFramebufferSizeCallback(window, resize);
}

void window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
