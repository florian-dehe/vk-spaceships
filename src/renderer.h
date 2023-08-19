#ifndef RENDERER_H_
#define RENDERER_H_

#include <GLFW/glfw3.h>
#include <cstdint>

#include "bgfx/bgfx.h"
#include "window.h"

struct Renderer {
    Renderer() = delete;
    ~Renderer() = delete;

    struct PosColorVertex {
        float x; // Position
        float y;
        uint32_t abgr; // Color

        static void init();
        static bgfx::VertexLayout layout;
    };

    struct PosTexCoordVertex {
        float x; // Position
        float y;
        int16_t u; // Texcoords
        int16_t v;

        static void init();
        static bgfx::VertexLayout layout;
    };

    static const uint16_t WIDTH;
    static const uint16_t HEIGHT;
    static const PosColorVertex SPRITE_COLOR_VTX[4];
    static const PosTexCoordVertex SPRITE_TEX_VTX[4];
    static const uint16_t SPRITE_IDX[6];

    static float viewMtx[16];
    static float projectionMtx[16];

    static bool Init(GameWindow&);
    static void Clear();
    static void Destroy();
};

#endif // RENDERER_H_
