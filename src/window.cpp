#include "window.h"
#include "renderer.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

// Constants
const char* GameWindow::TITLE = "Spaceships";

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

bool GameWindow::Init() {
    if (!glfwInit()) {
        std::cerr << "GLFW Init Failed !\n";
        return false;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "Vulkan not supported !\n";
        return false;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(Renderer::WIDTH, Renderer::HEIGHT, GameWindow::TITLE, glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        std::cerr << "GLFW Window creation failed !\n";
        return false;
    }
    return true;
}

void GameWindow::Destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GameWindow::SetResizeCallback(GLFWframebuffersizefun resize) {
    glfwSetFramebufferSizeCallback(window, resize);
}

void GameWindow::SetInputCallback(GLFWkeyfun input) {
    glfwSetKeyCallback(window, input);
}

bool GameWindow::Refresh() {
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
}

void* GameWindow::GetNativeWindow() {
    return (void*) (std::uintptr_t) glfwGetX11Window(window);
}

void* GameWindow::GetNativeDisplay() {
    return glfwGetX11Display();
}
