#ifndef INPUT_H_
#define INPUT_H_

#include "window.h"
#include <map>

class Game;

struct Input {
    Input() = delete;
    ~Input() = delete;

    struct InputAction {
        int key;
        int scancode;
        int mods;
        bool justPressed;
    };

    static std::map<int, InputAction> inputMap;
    static std::map<int, bool> inputJustPressedMap;

    static void Init(GameWindow&);

    static bool IsKeyPressed(int key);
    static bool IsKeyJustPressed(int key);
    static InputAction keyPressed(int key);
};

#endif // INPUT_H_
