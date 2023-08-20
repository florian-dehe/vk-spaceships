#ifndef GAME_H_
#define GAME_H_

#include <entt/entt.hpp>

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "components/transform.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/helper.hpp"
#include "window.h"
#include "input.h"

class Game {
public:
    Game() = default;
    ~Game() = default;

    // Initialize the game
    // Returns: false if the game can't be initialized
    bool Init();

    // Input at the begining of the game loop
    void OnInput();
    void Update(float dt);
    void Render();

    // Destroy remaining resources and finish game
    void Destroy();
private:
    const int BULLET_BORDER_Y = 1000;

    float viewMtx[16];

    // Textures
    bgfx::TextureHandle spaceshipTexture;
    bgfx::TextureHandle bulletTexture;

    // Entities
    entt::registry sceneRegistry;
    entt::entity player;

};


#endif // GAME_H_
