#ifndef RENDERER_H_
#define RENDERER_H_

#include "window.h"
#include <GLFW/glfw3.h>

struct renderer {
    static void resize_callback(GLFWwindow*, int, int);

    void init(const window&);
    void clear();
    void destroy();
};

#endif // RENDERER_H_
