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
    Game game;
    GameWindow win;

    std::cout << "Hello vk-spaceships!" << std::endl;

    if (!win.Init()) {
        std::cerr << "Init window failed!\n";
        return -1;
    }

    Input::Init(win);

    if (!Renderer::Init(win)) {
        std::cerr << "Init renderer failed!\n";
        return -1;
    }
    Renderer::Clear();

    if (!game.Init()) {
        return -1;
    }

    bool isRunning = true;
    double delta = 0.0;
    while (isRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();

        // Game Loop
        game.OnInput();
        game.Update(delta);
        game.Render();
        isRunning = win.Refresh();

        // Frame delta
        auto now = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count() / 1000000.f;
    }

    game.Destroy();
    win.Destroy();
    return 0;
}
