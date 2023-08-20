#include "renderer.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "game.h"
#include "resources.h"
#include "systems/sprite_renderer.h"
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

    // Init Modules
    SpriteRenderer::Init();

    return true;
}

void Renderer::Clear() {
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Renderer::Destroy() {
    // Destroy Modules
    SpriteRenderer::Destroy();

    bgfx::shutdown();
}
