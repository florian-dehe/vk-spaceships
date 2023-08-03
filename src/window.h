#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#define W_WIDTH 1600
#define W_HEIGHT 900

struct window {
    GLFWwindow *window = nullptr;
    void* nat_win = nullptr;
    void* nat_dis = nullptr;

    void init();
    void setResizeCallback(GLFWframebuffersizefun);
    void destroy();
};

#endif
