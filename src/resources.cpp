#include "resources.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"

#include <fstream>
#include <ios>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::string SHADER_FILE_PATH = "assets/shaders/";
std::string TEX_FILE_PATH = "assets/textures/";

bgfx::ShaderHandle loadShader(const char* filepath) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(SHADER_FILE_PATH + filepath);
    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }
    const bgfx::Memory* mem = bgfx::copy(data,fileSize+1);
    mem->data[mem->size-1] = '\0';
    return bgfx::createShader(mem);
}

bgfx::TextureHandle loadTexture(const char* filepath) {
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    int width, height, comp;
    uint8_t *data = stbi_load((TEX_FILE_PATH + filepath).c_str(), &width, &height, &comp, 0);
    if (data != nullptr) {
        std::cout << "Img: " << width*height << ", " << comp << std::endl;

        const bgfx::Memory* mem = bgfx::copy(data, width*height*comp);

        handle = bgfx::createTexture2D(
                uint16_t(width),
                uint16_t(height),
                false,
                1,
                bgfx::TextureFormat::RGBA8,
                BGFX_TEXTURE_MSAA_SAMPLE | BGFX_SAMPLER_POINT | BGFX_SAMPLER_UVW_BORDER,
                mem);

        stbi_image_free(data);
    }
    return handle;
}
