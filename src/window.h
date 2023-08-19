#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class GameWindow {
public:
    static const char* TITLE;

    bool Init();
    void Destroy();

    void SetResizeCallback(GLFWframebuffersizefun);
    void SetInputCallback(GLFWkeyfun input);
    bool Refresh();

    void* GetNativeWindow();
    void* GetNativeDisplay();
private:
    GLFWwindow *window = nullptr;
};

#endif
