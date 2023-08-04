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
    uint32_t abgr;
};


static PosColorVertex vertices[] = {
    {  0.5f,  0.5f, 0xffff0000 }, // Vertex 1: Blue
    {  0.5f, -0.5f, 0xff00ff00 }, // Vertex 2: Green
    { -0.5f, -0.5f, 0xff0000ff },  // Vertex 3: Red
    { -0.5f,  0.5f, 0xff0000ff }  // Vertex 4: Red
};

static uint16_t indices[] = {
    0, 1, 2,
    2, 3, 0
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

    rend.clear();

    bgfx::VertexLayout triangleLayout;
    triangleLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), triangleLayout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));

    bgfx::ShaderHandle vsh = loadShader("v_simple.bin");
    bgfx::ShaderHandle fsh = loadShader("f_simple.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    bgfx::touch(0);

    while(!glfwWindowShouldClose(win.window)) {
        glfwPollEvents();

        // Set view and projection matrix for view 0.
        float view[16];
        bx::mtxIdentity(view);

        float proj[16];
        bx::mtxOrtho(proj,
                    0.f, W_WIDTH,
                    0.f, W_HEIGHT,
                    -10.0f, 10.0f,
                    0.f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        // Set model matrix for rendering.
        float mtx[16];
        float translateMtx[16];
        bx::mtxTranslate(translateMtx, W_WIDTH / 2.f, W_HEIGHT / 2.f, 0.f);
        float scaleMtx[16];
        bx::mtxScale(scaleMtx, 500.f, 500.f, 1.f);
        bx::mtxMul(mtx, scaleMtx, translateMtx);

        bgfx::setTransform(mtx);

        bgfx::setState(
                BGFX_STATE_WRITE_R
                        | BGFX_STATE_WRITE_G
                        | BGFX_STATE_WRITE_B
                        | BGFX_STATE_WRITE_A
        );

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
    bgfx::destroy(program);

    rend.destroy();
    win.destroy();

    return 0;
}
