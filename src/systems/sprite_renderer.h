#ifndef SPRITE_RENDERER_H_
#define SPRITE_RENDERER_H_

#include "bgfx/bgfx.h"
struct Transform;
struct Mesh2D;
struct Material;

class SpriteRenderer {
public:
    SpriteRenderer() = delete;
    ~SpriteRenderer() = delete;

    struct PosTexCoordVertex {
        float x; // Position
        float y;
        int16_t u; // Texcoords
        int16_t v;

        static void init();
        static bgfx::VertexLayout layout;
    };

    static const PosTexCoordVertex SPRITE_TEX_VTX[4];
    static const uint16_t SPRITE_IDX[6];

    static bgfx::ProgramHandle DEFAULT_PROGRAM;
    static bgfx::UniformHandle DEFAULT_SAMPLER;
    static Mesh2D DEFAULT_MESH;

    static void Init();
    static void Destroy();
    static void Apply(const Transform&, const Mesh2D&, const Material&);
};


#endif // SPRITE_RENDERER_H_
