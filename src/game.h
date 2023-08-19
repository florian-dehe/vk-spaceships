#ifndef GAME_H_
#define GAME_H_

#include <entt/entt.hpp>

#include "bx/math.h"
#include "entt/entity/fwd.hpp"
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

    // Callback for window resize
    void OnWindowResize(int width, int height);

    // Game Loop
    // Returns: true if window should close
    bool Update(float dt);
    void Render();

    // Destroy remaining resources and finish game
    void Destroy();
private:
    GameWindow win;

    float viewMtx[16];

    // Entities
    entt::registry sceneRegistry;
    entt::entity sprite;
    int playerVelocity = 0;
    float playerSpeed = 550.f;
};


#endif // GAME_H_
