#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "bgfx/bgfx.h"

#include "window.h"
#include "renderer.h"

std::string SHADER_FILE_PATH = "assets/shaders/";

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

window win;
renderer rend;

int main(void) {
    std::cout << "Hello vk-spaceships!" << std::endl;

    win.init();

    if (!win.window) {
        std::cerr << "Init window failed!\n";
        return -1;
    }

    // bgfx stuff
    rend.init(win);
    win.setResizeCallback(renderer::resize_callback);

    rend.clear();

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

    while(!glfwWindowShouldClose(win.window)) {
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

    rend.destroy();
    win.destroy();

    return 0;
}
