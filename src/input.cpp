#include "input.h"
#include "game.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>
#include <vector>

std::map<int, Input::InputAction> Input::inputMap = std::map<int, Input::InputAction>();

void input_callback(GLFWwindow* _, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        Input::InputAction inputEvent = {
            key,
            scancode,
            mods,
            true
        };
        Input::inputMap.insert(std::make_pair(key, inputEvent));
    }
    else if (action == GLFW_RELEASE) {
        Input::inputMap.erase(key);
    }
}

void Input::Init(GameWindow& win) {
    win.SetInputCallback(input_callback);
}

bool Input::IsKeyPressed(int key) {
    bool pressed = inputMap.count(key) > 0;
    if (pressed) {
        InputAction& inputAction = inputMap.at(key);
        inputAction.justPressed = false;
    }
    return pressed;
}

bool Input::IsKeyJustPressed(int key) {
    bool pressed = inputMap.count(key) > 0;
    bool justPressed = false;
    if (pressed) {
        InputAction& inputAction = inputMap.at(key);
        justPressed = inputAction.justPressed;
        inputAction.justPressed = false;
    }
    return justPressed;
}

Input::InputAction Input::keyPressed(int key) {
    return inputMap.at(key);
}
