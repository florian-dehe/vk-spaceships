#ifndef SPRITE_H_
#define SPRITE_H_

#include "bgfx/bgfx.h"
#include "components/transform.h"
#include "entt/entt.hpp"

entt::entity createSprite(entt::registry&, const Transform,
                          const bgfx::TextureHandle);

#endif // SPRITE_H_
