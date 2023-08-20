#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "bx/math.h"
#include <array>

struct Movement {
    float speed;
    float velocity;
    bx::Vec3 axis = bx::Vec3(1.f, 0.f, 0.f);
    std::array<int, 4> boundaries = { 0, 0, 1600, 900 }; // left, bot, right, top
};


#endif // MOVEMENT_H_
