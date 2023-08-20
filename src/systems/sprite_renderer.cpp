#include "sprite_renderer.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"

#include "components/transform.h"
#include "components/mesh2d.h"
#include "components/material.h"
#include "resources.h"

bgfx::ProgramHandle SpriteRenderer::DEFAULT_PROGRAM;
bgfx::UniformHandle SpriteRenderer::DEFAULT_SAMPLER;
Mesh2D SpriteRenderer::DEFAULT_MESH;

const SpriteRenderer::PosTexCoordVertex SpriteRenderer::SPRITE_TEX_VTX[4] = {
    {  0.5f,  0.5f, 0x7fff,      0 }, // right, top
    {  0.5f, -0.5f, 0x7fff, 0x7fff }, // right, bottom
    { -0.5f, -0.5f,      0, 0x7fff }, // left, bottom
    { -0.5f,  0.5f,      0,      0 }  // left, top
};

const uint16_t SpriteRenderer::SPRITE_IDX[6] = {
    0, 1, 2,
    2, 3, 0
};


bgfx::VertexLayout SpriteRenderer::PosTexCoordVertex::layout;
void SpriteRenderer::PosTexCoordVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
        .end();
}

void SpriteRenderer::Init() {
    PosTexCoordVertex::init();

    DEFAULT_MESH = {
        bgfx::createVertexBuffer(
            bgfx::makeRef(SPRITE_TEX_VTX, sizeof(SPRITE_TEX_VTX)),
            PosTexCoordVertex::layout),
        bgfx::createIndexBuffer(bgfx::makeRef(SPRITE_IDX, sizeof(SPRITE_IDX)))
    };

    DEFAULT_PROGRAM = bgfx::createProgram(
        loadShader("v_tex.bin"), loadShader("f_tex.bin"), true);

    DEFAULT_SAMPLER = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
}


void SpriteRenderer::Destroy() {
    bgfx::destroy(DEFAULT_MESH.vbh);
    bgfx::destroy(DEFAULT_MESH.ibh);

    bgfx::destroy(DEFAULT_PROGRAM);
    bgfx::destroy(DEFAULT_SAMPLER);
}

void SpriteRenderer::Apply(const Transform& t, const Mesh2D& m, const Material& mat) {
    // Transform: model mtx for rendering
    float mtx[16];
    float translateMtx[16];
    bx::mtxTranslate(translateMtx, t.position.x, t.position.y, t.position.z);
    float rotateMtx[16];
    bx::mtxRotateXYZ(rotateMtx, t.rotation.x, t.rotation.y, t.rotation.z);
    float scaleMtx[16];
    bx::mtxScale(scaleMtx, t.scale.x, t.scale.y, t.scale.z);

    bx::mtxMul(mtx, scaleMtx, rotateMtx);
    bx::mtxMul(mtx, mtx, translateMtx);
    bgfx::setTransform(mtx);

    // Mesh2D
    bgfx::setVertexBuffer(0, m.vbh);
    bgfx::setIndexBuffer(m.ibh);

    // Material
    bgfx::setTexture(0, mat.texture.sampler, mat.texture.handle);

    // Draw
    bgfx::setState(mat.renderState);
    bgfx::submit(0, mat.program);
}
