#include "game.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "input.h"
#include "renderer.h"
#include "resources.h"

#include "components/transform.h"
#include "components/material.h"
#include "components/mesh2d.h"

#include <GLFW/glfw3.h>
#include <iostream>

bool Game::Init() {
    if (!win.Init()) {
        std::cerr << "Init window failed!\n";
        return false;
    }

    Input::Init(win);

    if (!Renderer::Init(win)) {
        std::cerr << "Init renderer failed!\n";
        return false;
    }
    Renderer::Clear();

    // Sprite entity
    sprite = sceneRegistry.create();

    Mesh2D spriteMesh = {
        bgfx::createVertexBuffer(
            bgfx::makeRef(Renderer::SPRITE_TEX_VTX, sizeof(Renderer::SPRITE_TEX_VTX)),
            Renderer::PosTexCoordVertex::layout),
        bgfx::createIndexBuffer(
            bgfx::makeRef(Renderer::SPRITE_IDX, sizeof(Renderer::SPRITE_IDX)))
    };
    sceneRegistry.emplace<Mesh2D>(sprite, spriteMesh);

    Transform spriteTransform = {
        bx::Vec3(Renderer::WIDTH / 2.f,
                 Renderer::HEIGHT / 10.f,
                 0.f),
        bx::Vec3(0.f, 0.f, 0.f),
        bx::Vec3(100.f, 100.f, 0.f)
    };
    sceneRegistry.emplace<Transform>(sprite, spriteTransform);

    // Shaders
    bgfx::ShaderHandle vsh = loadShader("v_tex.bin");
    bgfx::ShaderHandle fsh = loadShader("f_tex.bin");
    Material::Texture spriteTexture = {
        loadTexture("spaceship.png"),
        bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler)
    };
    Material spriteMaterial = {
        bgfx::createProgram(vsh, fsh, true),
        spriteTexture,
        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA
    };
    sceneRegistry.emplace<Material>(sprite, spriteMaterial);

    bgfx::touch(0);

    return true;
}

void Game::OnWindowResize(int width, int height) {

}

void Game::OnInput() {
    if (Input::IsKeyPressed(GLFW_KEY_RIGHT) || Input::IsKeyPressed(GLFW_KEY_D)) {
        playerVelocity += 1;
    }
    else if (Input::IsKeyPressed(GLFW_KEY_LEFT) || Input::IsKeyPressed(GLFW_KEY_Q)) {
        playerVelocity -= 1;
    }
    else if (Input::IsKeyJustPressed(GLFW_KEY_SPACE)) {
        std::cout << "shoot" << std::endl;
    }
}

bool Game::Update(float dt) {
    // Set view and projection matrix for view 0.
    bx::mtxIdentity(viewMtx); /* TODO: Make it move depding on user input */
    bgfx::setViewTransform(0, viewMtx, Renderer::projectionMtx);

    if (playerVelocity != 0) {
        sceneRegistry.patch<Transform>(sprite, [&, this](Transform& playerTransform) {
            playerTransform.position.x += playerVelocity * playerSpeed * dt;
            std::cout << "Player Transform: " << std::endl;
            std::cout << playerTransform.position.x << std::endl;
            std::cout << dt << std::endl;

            playerVelocity = 0;
        });
    }

    return win.Refresh();
}

void Game::Render() {
    auto view = sceneRegistry.view<Transform, Mesh2D, Material>();
    view.each([this](const auto entity, const Transform& t, const Mesh2D& m, const Material& mat) {
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

    });

    bgfx::frame();
}

void Game::Destroy() {
    for (entt::entity meshEntity: sceneRegistry.group<Mesh2D>()) {
       auto& mesh = sceneRegistry.get<Mesh2D>(meshEntity);
       bgfx::destroy(mesh.vbh);
       bgfx::destroy(mesh.ibh);
    }

    for (entt::entity materialEntity: sceneRegistry.group<Material>()) {
       auto& mat = sceneRegistry.get<Material>(materialEntity);
       bgfx::destroy(mat.program);
       bgfx::destroy(mat.texture.handle);
       bgfx::destroy(mat.texture.sampler);
    }

    sceneRegistry.destroy(sprite);

    Renderer::Destroy();
    win.Destroy();
}
