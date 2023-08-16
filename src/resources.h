#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "bgfx/bgfx.h"

bgfx::ShaderHandle loadShader(const char* filepath);

bgfx::TextureHandle loadTexture(const char* filepath);

#endif // RESOURCES_H_
