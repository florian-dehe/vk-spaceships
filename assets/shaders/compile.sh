#!/usr/bin/env sh

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f v_simple.sc \
    -o v_simple.bin \
    --platform linux \
    --profile spirv \
    --type vertex \
    --varyingdef varying.def.sc \
    -i ../../libs/bgfx.cmake/bgfx/src

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f f_simple.sc \
    -o f_simple.bin \
    --platform linux \
    --profile spirv \
    --type fragment \
    --varyingdef varying.def.sc \
    -i ../../libs/bgfx.cmake/bgfx/src

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f v_tex.sc \
    -o v_tex.bin \
    --platform linux \
    --profile spirv \
    --type vertex \
    --varyingdef varying.def.sc \
    -i ../../libs/bgfx.cmake/bgfx/src

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f f_tex.sc \
    -o f_tex.bin \
    --platform linux \
    --profile spirv \
    --type fragment \
    --varyingdef varying.def.sc \
    -i ../../libs/bgfx.cmake/bgfx/src
