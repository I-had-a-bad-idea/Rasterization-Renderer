
#include "Image_loader.h"

std::vector<unsigned char> ImageLoader::png_file_to_bytes(const std::string& filePath) {
    // Load PNG using raylib
    Image image = LoadImage(filePath.c_str());
    if (image.data == nullptr) {
        throw std::runtime_error("Failed to load PNG: " + filePath);
    }

    int width = image.width;
    int height = image.height;

    // Convert to 24-bit RGB
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8);

    unsigned char* pixels = (unsigned char*)image.data;

    // Build buffer: [w0][w1][h0][h1] + BGR pixels
    std::vector<unsigned char> buffer;
    buffer.resize(4 + width * height * 3);

    buffer[0] = width & 0xFF;
    buffer[1] = (width >> 8) & 0xFF;
    buffer[2] = height & 0xFF;
    buffer[3] = (height >> 8) & 0xFF;

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

    UnloadImage(image);

    return buffer;
}