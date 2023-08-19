#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "bgfx/bgfx.h"
#include <cstdint>

struct Material {
    struct Texture {
        bgfx::TextureHandle handle;
        bgfx::UniformHandle sampler;
    };

    bgfx::ProgramHandle program;
    Texture texture;
    uint64_t renderState;
};

#endif // MATERIAL_H_
