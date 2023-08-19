#ifndef MESH2D_H_
#define MESH2D_H_

#include "bgfx/bgfx.h"

struct Mesh2D {
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
};

#endif // MESH2D_H_
