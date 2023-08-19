#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "bx/math.h"

struct Transform {
    bx::Vec3 position;
    bx::Vec3 rotation;
    bx::Vec3 scale;
};

#endif // TRANSFORM_H_
