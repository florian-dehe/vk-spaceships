#!/usr/bin/env sh

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f v_simple.sc \
    -o v_simple.bin \
    --platform linux \
    --profile spirv \
    --type vertex \
    --varyingdef varying.def.sc \
    --verbose \
    -i ../../libs/bgfx.cmake/bgfx/src

../../build/libs/bgfx.cmake/cmake/bgfx/shaderc \
    -f f_simple.sc \
    -o f_simple.bin \
    --platform linux \
    --profile spirv \
    --type fragment \
    --varyingdef varying.def.sc \
    --verbose \
    -i ../../libs/bgfx.cmake/bgfx/src
