#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <algorithm>

struct float2 {
    float x, y;
    float2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct float3 {
    float r, g, b;
    float3(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {}
};

// Helper: Check if a point is inside a triangle 
bool PointInTriangle(const float2& a, const float2& b, const float2& c, const float2& p) {
    auto sign = [](const float2& p1, const float2& p2, const float2& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - 
               (p2.x - p3.x) * (p1.y - p3.y);
    };

    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

// Save as BMP
void WriteImageToFile(const std::vector<float3>& image, int width, int height, const std::string& name) {
    std::ofstream writer(name + ".bmp", std::ios::binary);
    if (!writer) throw std::runtime_error("Failed to open file for writing");

    uint32_t fileSize = 14 + 40 + width * height * 4;
    uint32_t dataOffset = 14 + 40;
    uint32_t dibHeaderSize = 40;
    uint16_t planes = 1;
    uint16_t bitsPerPixel = 8 * 4;
    uint32_t compression = 0;
    uint32_t dataSize = width * height * 4;

    writer.write("BM", 2);
    writer.write(reinterpret_cast<char*>(&fileSize), 4);
    uint32_t reserved = 0;
    writer.write(reinterpret_cast<char*>(&reserved), 4);
    writer.write(reinterpret_cast<char*>(&dataOffset), 4);

    writer.write(reinterpret_cast<char*>(&dibHeaderSize), 4);
    writer.write(reinterpret_cast<char*>(&width), 4);
    writer.write(reinterpret_cast<char*>(&height), 4);
    writer.write(reinterpret_cast<char*>(&planes), 2);
    writer.write(reinterpret_cast<char*>(&bitsPerPixel), 2);
    writer.write(reinterpret_cast<char*>(&compression), 4);
    writer.write(reinterpret_cast<char*>(&dataSize), 4);
    uint32_t ppm = 2835;
    writer.write(reinterpret_cast<char*>(&ppm), 4);
    writer.write(reinterpret_cast<char*>(&ppm), 4);
    uint32_t paletteColors = 0;
    writer.write(reinterpret_cast<char*>(&paletteColors), 4);
    uint32_t importantColors = 0;
    writer.write(reinterpret_cast<char*>(&importantColors), 4);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const float3& col = image[y * width + x];
            uint8_t r = static_cast<uint8_t>(std::clamp(col.r * 255.0f, 0.0f, 255.0f));
            uint8_t g = static_cast<uint8_t>(std::clamp(col.g * 255.0f, 0.0f, 255.0f));
            uint8_t b = static_cast<uint8_t>(std::clamp(col.b * 255.0f, 0.0f, 255.0f));
            uint8_t pad = 0;
            writer.write(reinterpret_cast<char*>(&b), 1);
            writer.write(reinterpret_cast<char*>(&g), 1);
            writer.write(reinterpret_cast<char*>(&r), 1);
            writer.write(reinterpret_cast<char*>(&pad), 1);
        }
    }
}

// Create the test image with a blue triangle
void CreateTestImage() {
    const int width = 64;
    const int height = 64;
    std::vector<float3> image(width * height, float3(1.0f, 1.0f, 1.0f)); // white background

    float2 a(0.2f * width, 0.2f * height);
    float2 b(0.7f * width, 0.4f * height);
    float2 c(0.4f * width, 0.8f * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float2 p(x, y);
            if (PointInTriangle(a, b, c, p)) {
                image[y * width + x] = float3(0.0f, 0.0f, 1.0f); // blue
            }
        }
    }

    WriteImageToFile(image, width, height, "art");
}

int main() {
    try {
        CreateTestImage();
        std::cout << "Image created successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
