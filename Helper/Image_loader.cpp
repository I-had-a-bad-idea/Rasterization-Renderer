#include "Image_loader.h"

std::vector<unsigned char> ImageLoader::png_file_to_bytes(const std::string& filePath) {
    // Load PNG using SDL_image
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load PNG: " + filePath + " SDL Error: " + IMG_GetError());
    }

    int width = surface->w;
    int height = surface->h;

    // Convert surface to 24-bit RGB if needed
    SDL_Surface* rgbSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(surface);
    
    if (!rgbSurface) {
        throw std::runtime_error("Failed to convert image format: " + std::string(SDL_GetError()));
    }

    // Build buffer: [w0][w1][h0][h1] + BGR pixels
    std::vector<unsigned char> buffer;
    buffer.resize(4 + width * height * 3);

    buffer[0] = width & 0xFF;
    buffer[1] = (width >> 8) & 0xFF;
    buffer[2] = height & 0xFF;
    buffer[3] = (height >> 8) & 0xFF;

    // Get pixel data
    unsigned char* pixels = static_cast<unsigned char*>(rgbSurface->pixels);
    size_t index = 4;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            unsigned char r = pixels[i + 0];
            unsigned char g = pixels[i + 1];
            unsigned char b = pixels[i + 2];

            // MeshTexture::CreateFromBytes expects BGR
            buffer[index + 0] = b;
            buffer[index + 1] = r;
            buffer[index + 2] = g;
            index += 3;
        }
    }

    SDL_FreeSurface(rgbSurface);

    return buffer;
}