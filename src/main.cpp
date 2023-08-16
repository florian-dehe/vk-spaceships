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
#include "resources.h"

window win;

int main(void) {
    std::cout << "Hello vk-spaceships!" << std::endl;

    win.init();

    if (!win.window) {
        std::cerr << "Init window failed!\n";
        return -1;
    }

    Renderer::init(win);

    Renderer::clear();

    // Mesh
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(Renderer::SPRITE_TEX_VTX, sizeof(Renderer::SPRITE_TEX_VTX)),
        Renderer::PosTexCoordVertex::layout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(Renderer::SPRITE_IDX, sizeof(Renderer::SPRITE_IDX))
    );

    // Shaders
    bgfx::ShaderHandle vsh = loadShader("v_tex.bin");
    bgfx::ShaderHandle fsh = loadShader("f_tex.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    // Texture
    bgfx::TextureHandle texture = loadTexture("spaceship.png");
    bgfx::UniformHandle s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

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

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::setTexture(0, s_texColor, texture);

        bgfx::setState(0
                | BGFX_STATE_WRITE_RGB
                | BGFX_STATE_WRITE_A
                | BGFX_STATE_MSAA
        );

        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::destroy(s_texColor);
    bgfx::destroy(texture);
    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
    bgfx::destroy(program);

    Renderer::destroy();
    win.destroy();

    return 0;
}
