#include "renderer.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"

void Renderer::resize_callback(GLFWwindow* _, int width, int height) {
    bgfx::reset((uint32_t) width, (uint32_t) height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Renderer::init(window& win) {
    // No multithreading
    bgfx::renderFrame();

    bgfx::Init initBgfx;
    initBgfx.type = bgfx::RendererType::Vulkan;

    initBgfx.platformData.nwh = win.nat_win;
    initBgfx.platformData.ndt = win.nat_dis;

    initBgfx.resolution.width = W_WIDTH;
    initBgfx.resolution.height = W_HEIGHT;
    initBgfx.resolution.reset = BGFX_RESET_VSYNC;

    if(!bgfx::init(initBgfx)) {
        std::cerr << "BGFX init failed !\n";
        return;
    }
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);
    win.setResizeCallback(Renderer::resize_callback);

    // Init structures
    PosColorVertex::init();
    PosTexCoordVertex::init();
}

void Renderer::clear() {
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Renderer::destroy() {
    bgfx::shutdown();
}

bgfx::VertexLayout Renderer::PosColorVertex::layout;
void Renderer::PosColorVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, true)
        .end();
}

bgfx::VertexLayout Renderer::PosTexCoordVertex::layout;
void Renderer::PosTexCoordVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
        .end();
}

const Renderer::PosColorVertex Renderer::SPRITE_COLOR_VTX[4] = {
    {  0.5f,  0.5f, 0xffff0000 }, // right, top; blue
    {  0.5f, -0.5f, 0xff00ff00 }, // right, bottom; green
    { -0.5f, -0.5f, 0xff0000ff }, // left, bottom; red
    { -0.5f,  0.5f, 0xff0000ff }  // left, top; red
};

const Renderer::PosTexCoordVertex Renderer::SPRITE_TEX_VTX[4] = {
    {  0.5f,  0.5f, 0x7fff,      0 }, // right, top
    {  0.5f, -0.5f, 0x7fff, 0x7fff }, // right, bottom
    { -0.5f, -0.5f,      0, 0x7fff }, // left, bottom
    { -0.5f,  0.5f,      0,      0 }  // left, top
};

const uint16_t Renderer::SPRITE_IDX[6] = {
    0, 1, 2,
    2, 3, 0
};
