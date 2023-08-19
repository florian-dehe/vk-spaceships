#include "renderer.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "game.h"
#include "window.h"

const uint16_t Renderer::WIDTH = 1600;
const uint16_t Renderer::HEIGHT = 900;

float Renderer::viewMtx[16];
float Renderer::projectionMtx[16];

void resize_callback(GLFWwindow* _, int width, int height) {
    bgfx::reset((uint32_t) width, (uint32_t) height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

bool Renderer::Init(GameWindow& win) {
    // No multithreading
    bgfx::renderFrame();

    bgfx::Init initBgfx;
    initBgfx.type = bgfx::RendererType::Vulkan;

    initBgfx.platformData.nwh = win.GetNativeWindow();
    initBgfx.platformData.ndt = win.GetNativeDisplay();

    initBgfx.resolution.width = WIDTH;
    initBgfx.resolution.height = HEIGHT;
    initBgfx.resolution.reset = BGFX_RESET_VSYNC;

    if(!bgfx::init(initBgfx)) {
        std::cerr << "BGFX init failed !\n";
        return false;
    }
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);
    win.SetResizeCallback(resize_callback);

    // Recalculate the projection matrix
    bx::mtxOrtho(projectionMtx,
                0.f, WIDTH,
                0.f, HEIGHT,
                -10.0f, 10.0f,
                0.f, bgfx::getCaps()->homogeneousDepth);

    // Init structures
    PosColorVertex::init();
    PosTexCoordVertex::init();

    return true;
}

void Renderer::Clear() {
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Renderer::Destroy() {
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
