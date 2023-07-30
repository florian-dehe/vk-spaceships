#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "bgfx/bgfx.h"

#define W_WIDTH 1600
#define W_HEIGHT 900

std::string SHADER_FILE_PATH = "assets/shaders/";

void resize_callback(GLFWwindow *window, int width, int height) {
  bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
  bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

bgfx::ShaderHandle loadShader(const char* filepath) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(SHADER_FILE_PATH + filepath);
    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }
    const bgfx::Memory* mem = bgfx::copy(data,fileSize+1);
    mem->data[mem->size-1] = '\0';
    return bgfx::createShader(mem);
}

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
};


static PosColorVertex vertices[] = {
    {  0.0f,  0.5f, 0.0f, 0xffff0000 }, // Vertex 1: Blue
    {  0.5f, -0.5f, 0.0f, 0xff00ff00 }, // Vertex 2: Green
    { -0.5f, -0.5f, 0.0f, 0xff0000ff }  // Vertex 3: Red
};

int main(void) {
    GLFWwindow *window;

    std::cout << "Hello vk-spaceships!" << std::endl;

    if (!glfwInit()) {
        std::cerr << "GLFW Init Failed !\n";
        return -1;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "Vulkan not supported !\n";
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Spaceships", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW Window creation failed !\n";
        return -1;
    }

    // bgfx stuff
    bgfx::renderFrame();

    bgfx::Init initBgfx;
    initBgfx.type = bgfx::RendererType::Vulkan;

    initBgfx.platformData.nwh = (void*) (std::uintptr_t) glfwGetX11Window(window);
    initBgfx.platformData.ndt = glfwGetX11Display();

    initBgfx.resolution.width = W_WIDTH;
    initBgfx.resolution.height = W_HEIGHT;
    initBgfx.resolution.reset = BGFX_RESET_VSYNC;

    if(!bgfx::init(initBgfx)) {
        std::cerr << "BGFX init failed !\n";
        return -1;
    }
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    glfwSetFramebufferSizeCallback(window, resize_callback);

    // Clear
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::VertexLayout triangleLayout;
    triangleLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), triangleLayout);

    bgfx::ShaderHandle vsh = loadShader("v_simple.bin");
    bgfx::ShaderHandle fsh = loadShader("f_simple.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    bgfx::touch(0);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const bx::Vec3 at  = { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, 2.0f };

        // Set view and projection matrix for view 0.
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj,
                    60.0f,
                    float(W_WIDTH)/float(W_HEIGHT),
                    0.1f, 100.0f,
                    bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);

        // position x,y,z
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        bgfx::setState(
                BGFX_STATE_WRITE_R
                        | BGFX_STATE_WRITE_G
                        | BGFX_STATE_WRITE_B
                        | BGFX_STATE_WRITE_A
        );

        bgfx::setVertexBuffer(0, vbh);
        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::destroy(vbh);
    bgfx::destroy(program);
    bgfx::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
