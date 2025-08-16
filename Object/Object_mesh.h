#ifndef OBJECT_MESH_H
#define OBJECT_MESH_H

#include "Vector.h"
#include <vector>
#include <cstdint>
#include <stdexcept>

class ObjectMesh{
    public:
        std::vector<float3> Vertices;
        std::vector<float3> Normals;
        std::vector<float2> Texture_cords;

        ObjectMesh() = default;
        ObjectMesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<float2> texture_cords);
};

class MeshTexture {
public:
    int Width;
    int Height;
    int wscale;
    int hscale;
    std::vector<float3> image; // Flattened 2D array (row-major)

    MeshTexture() = default;

    MeshTexture(const std::vector<float3>& img, int width, int height);
    float3 Sample(float u, float v) const;

    static MeshTexture CreateFromBytes(const std::vector<uint8_t>& bytes);
};

class ObjectShader {
public:
    virtual ~ObjectShader() = default;
    virtual float3 PixelColor(float2 tex_coord) const = 0;
};

#endif