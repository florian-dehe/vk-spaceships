#include "renderer.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"

void renderer::resize_callback(GLFWwindow* _, int width, int height) {
    bgfx::reset((uint32_t) width, (uint32_t) height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void renderer::init(const window& win) {
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
}

void renderer::clear() {
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void renderer::destroy() {
    bgfx::shutdown();
}
