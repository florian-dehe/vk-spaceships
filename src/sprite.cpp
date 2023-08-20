#include "sprite.h"
#include "bgfx/bgfx.h"

#include "renderer.h"
#include "components/mesh2d.h"
#include "components/transform.h"
#include "components/material.h"
#include "resources.h"
#include "systems/sprite_renderer.h"

entt::entity createSprite(entt::registry& reg, const Transform t,
                          const bgfx::TextureHandle texture) {

    entt::entity sprite = reg.create();

    reg.emplace<Mesh2D>(sprite, SpriteRenderer::DEFAULT_MESH);
    reg.emplace<Transform>(sprite, t);

    // Shaders
    Material::Texture spriteTexture = {
        texture,
        SpriteRenderer::DEFAULT_SAMPLER
    };
    Material spriteMaterial = {
        SpriteRenderer::DEFAULT_PROGRAM,
        spriteTexture,
        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA
    };
    reg.emplace<Material>(sprite, spriteMaterial);

    return sprite;
}
