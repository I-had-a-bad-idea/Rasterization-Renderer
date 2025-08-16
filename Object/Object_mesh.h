#ifndef OBJECT_MESH_H
#define OBJECT_MESH_H

#include "Vector.h"
#include <vector>

class ObjectMesh{
    public:
        std::vector<float3> Vertices;
        std::vector<float3> Normals;
        std::vector<float2> Texture_cords;

        ObjectMesh() = default;
        ObjectMesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<float2> texture_cords);
};

class Texture {
public:
    int Width;
    int Height;
    int wscale;
    int hscale;
    std::vector<float3> image; // Flattened 2D array (row-major)

    Texture(const std::vector<float3>& img, int width, int height)
        : Width(width), Height(height), image(img)
    {
        if (width <= 0 || height <= 0 || img.size() != static_cast<size_t>(width * height)) {
            throw std::exception("Invalid image dimensions or data");
        }
        wscale = width - 1;
        hscale = height - 1;
    }

    inline float3 Sample(float u, float v) const {
        // Wrap UVs into [0,1)
        float fu = u - std::floor(u);
        float fv = v - std::floor(v);

        int x = static_cast<int>(fu * wscale);
        int y = static_cast<int>(fv * hscale);

        return image[y * Width + x]; // row-major indexing
    }

    static Texture CreateFromBytes(const std::vector<uint8_t>& bytes) {
        if (bytes.size() < 4) {
            throw std::exception("Byte array too small for header");
        }

        int width = bytes[0] | (bytes[1] << 8);
        int height = bytes[2] | (bytes[3] << 8);

        std::vector<float3> img(width * height);

        size_t index = 4; // start after width/height header
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (index + 2 >= bytes.size()) {
                    throw std::exception("Not enough pixel data in byte array");
                }
                float r = bytes[index + 1] / 255.0f;
                float g = bytes[index + 2] / 255.0f;
                float b = bytes[index + 0] / 255.0f;
                img[y * width + x] = float3(r, g, b);
                index += 3;
            }
        }

        return Texture(img, width, height);
    }
};

class Shader{
    public:
        virtual float3 PixelColor(float2 text_coord){};
};

#endif