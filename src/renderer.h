#ifndef RENDERER_H_
#define RENDERER_H_

#include <cstdint>

#include "window.h"

struct Renderer {
    Renderer() = delete;
    ~Renderer() = delete;

    static const uint16_t WIDTH;
    static const uint16_t HEIGHT;

    static float viewMtx[16];
    static float projectionMtx[16];

    static bool Init(GameWindow&);
    static void Clear();
    static void Destroy();
};

#endif // RENDERER_H_
