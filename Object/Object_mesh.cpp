
#include "Object_mesh.h"

MeshTexture::MeshTexture(const std::vector<float3>& img, int width, int height)
    : Width(width), Height(height), image(img)
{
    if (width <= 0 || height <= 0 || img.size() != static_cast<size_t>(width * height)) {
        throw std::runtime_error("Invalid image dimensions or data");
    }
    wscale = width - 1;
    hscale = height - 1;
}

float3 MeshTexture::Sample(float u, float v) const {
    // Wrap UVs into [0,1)
    float fu = u - std::floor(u);
    float fv = v - std::floor(v);

    int x = static_cast<int>(fu * (Width - 1));
    int y = static_cast<int>(fv * (Height - 1));

    // Clamp to valid range
    if (x < 0) x = 0;
    if (x >= Width) x = Width - 1;
    if (y < 0) y = 0;
    if (y >= Height) y = Height - 1;

    return image[y * Width + x];
}

MeshTexture MeshTexture::CreateFromBytes(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < 4) {
        throw std::runtime_error("Byte array too small for header");
    }

    int width = bytes[0] | (bytes[1] << 8);
    int height = bytes[2] | (bytes[3] << 8);

    std::vector<float3> img(width * height);

    size_t index = 4; // start after width/height header
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (index + 2 >= bytes.size()) {
                throw std::runtime_error("Not enough pixel data in byte array");
            }
            float r = bytes[index + 1] / 255.0f;
            float g = bytes[index + 2] / 255.0f;
            float b = bytes[index + 0] / 255.0f;
            img[y * width + x] = float3(r, g, b);
            index += 3;
        }
    }

    return MeshTexture(img, width, height);
}


ObjectMesh::ObjectMesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<float2> texture_cords){
    Vertices = vertices;
    Normals = normals;
    Texture_cords = texture_cords;
}
