#include "game.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "components/material.h"
#include "components/mesh2d.h"
#include "components/movement.h"
#include "entt/entity/fwd.hpp"
#include "input.h"
#include "renderer.h"
#include "resources.h"

#include "components/transform.h"
#include "sprite.h"
#include "systems/sprite_renderer.h"

#include <iostream>

bool Game::Init() {
    // Init textures
    spaceshipTexture = loadTexture("spaceship.png");
    bulletTexture = loadTexture("bullet.png");


    // Sprite entity
    Transform spriteTransform = {
        bx::Vec3(Renderer::WIDTH / 2.f,
                 Renderer::HEIGHT / 12.f,
                 0.f),
        bx::Vec3(0.f, 0.f, 0.f),
        bx::Vec3(100.f, 100.f, 0.f)
    };
    player = createSprite(sceneRegistry, spriteTransform, spaceshipTexture);

    Movement playerMovement = {
        550.f,
        0.f,
        bx::Vec3(1.0f, 0.f, 0.f)
    };
    sceneRegistry.emplace<Movement>(player, playerMovement);

    bgfx::touch(0);

    return true;
}

void Game::OnInput() {
    sceneRegistry.patch<Movement>(player, [](Movement& playerMovement) {
        if (Input::IsKeyPressed(GLFW_KEY_RIGHT) || Input::IsKeyPressed(GLFW_KEY_D)) {
            playerMovement.velocity += 1.f;
        }
        else if (Input::IsKeyPressed(GLFW_KEY_LEFT) || Input::IsKeyPressed(GLFW_KEY_Q)) {
            playerMovement.velocity -= 1.f;
        }
    });
    if (Input::IsKeyJustPressed(GLFW_KEY_SPACE)) {
        Transform playerTransform = sceneRegistry.get<Transform>(player);
        Transform bulletTransform = {
            playerTransform.position,
            bx::Vec3(0.f, 0.f, 0.f),
            bx::Vec3(32.f, 60.f, 0.f)
        };
        auto bullet = createSprite(sceneRegistry, bulletTransform, bulletTexture);
        Movement bulletMovement = {
            750.f,
            1.f,
            bx::Vec3(0.0f, 1.f, 0.f),
            { 0, 0, 1600, BULLET_BORDER_Y }
        };
        sceneRegistry.emplace<Movement>(bullet, bulletMovement);
    }
}

void Game::Update(float dt) {

    auto movables = sceneRegistry.view<Transform, const Movement>();
    movables.each([&](Transform& t, const Movement& m) {
        const float halfScaleX = (t.scale.x / 2.f);
        t.position.x += m.velocity * m.speed * m.axis.x * dt;
        if (t.position.x < m.boundaries[0] + halfScaleX) {
            t.position.x = m.boundaries[0] + halfScaleX;
        }
        else if (t.position.x > m.boundaries[2] - halfScaleX) {
            t.position.x = m.boundaries[2] - halfScaleX;
        }

        const float halfScaleY = (t.scale.y / 2.f);
        t.position.y += m.velocity * m.speed * m.axis.y * dt;
        if (t.position.y < m.boundaries[1] + halfScaleY) {
            t.position.y = m.boundaries[1] + halfScaleY;
        }
        else if (t.position.y > m.boundaries[3] - halfScaleY) {
            t.position.y = m.boundaries[3] - halfScaleY;
        }
    });

    for (auto [entity, transform] : sceneRegistry.view<Transform>().each()) {
        if (entity != player && transform.position.y >= BULLET_BORDER_Y - transform.scale.y / 2) {
            sceneRegistry.destroy(entity);
        }
    }
    sceneRegistry.patch<Movement>(player, [](Movement& m){ m.velocity = 0.f; });
}

void Game::Render() {
    // Set view and projection matrix for view 0.
    bx::mtxIdentity(viewMtx);
    bgfx::setViewTransform(0, viewMtx, Renderer::projectionMtx);

    sceneRegistry.view<Transform, Mesh2D, Material>()
        .each(SpriteRenderer::Apply);

    bgfx::frame();
}

void Game::Destroy() {
    bgfx::destroy(spaceshipTexture);
    bgfx::destroy(bulletTexture);

    sceneRegistry.destroy(player);

    Renderer::Destroy();
}
