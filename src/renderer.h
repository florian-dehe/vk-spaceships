#ifndef RENDERER_H_
#define RENDERER_H_

#include "bgfx/bgfx.h"
#include "window.h"
#include <GLFW/glfw3.h>


class Renderer {
private:
    Renderer() = default;
    ~Renderer() = default;
public:
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

    // Constants
    static const PosColorVertex SPRITE_COLOR_VTX[4];
    static const PosTexCoordVertex SPRITE_TEX_VTX[4];
    static const uint16_t SPRITE_IDX[6];

    // Callbacks
    static void resize_callback(GLFWwindow*, int, int);

    // Lifecycle
    static void init(window&);
    static void clear();
    static void destroy();
};

#endif // RENDERER_H_
