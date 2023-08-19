#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "bgfx/bgfx.h"

#include "window.h"
#include "renderer.h"
#include "resources.h"
#include "game.h"

int main(void) {
    std::cout << "Hello vk-spaceships!" << std::endl;

    bool isRunning = true;
    double delta = 0.0;
    Game game;

    if (!game.Init()) {
        return -1;
    }

    while (isRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();

        // Game Loop
        game.OnInput();
        isRunning = game.Update(delta);
        game.Render();

        // Frame delta
        auto now = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count() / 1000000.f;
    }

    game.Destroy();
    return 0;
}
