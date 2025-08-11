#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "Maths.cpp"


void write_image_to_file(const std::vector<float3>& image, int width, int height, const std::string& name) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid image dimensions");
    }

    if (image.size() != static_cast<size_t>(width * height)) {
        throw std::invalid_argument("Image buffer size doesn't match dimensions");
    }
    std::ofstream writer(name + ".bmp", std::ios::binary);
    if (!writer) {
        throw std::runtime_error("Failed to create output file");
    }

    uint32_t fileSize = 14 + 40 + (width * height * 4);
    uint32_t dataOffset = 14 + 40;
    uint32_t dibHeaderSize = 40;
    uint16_t planes = 1;
    uint16_t bitsPerPixel = 8 * 4;
    uint32_t compression = 0;
    uint32_t dataSize = width * height * 4;

    // --- BMP Header ---
    writer.write("BM", 2);
    writer.write(reinterpret_cast<char*>(&fileSize), 4);
    uint32_t reserved = 0;
    writer.write(reinterpret_cast<char*>(&reserved), 4);
    writer.write(reinterpret_cast<char*>(&dataOffset), 4);

    // --- DIB Header ---
    writer.write(reinterpret_cast<char*>(&dibHeaderSize), 4);
    writer.write(reinterpret_cast<char*>(&width), 4);
    writer.write(reinterpret_cast<char*>(&height), 4);
    writer.write(reinterpret_cast<char*>(&planes), 2);
    writer.write(reinterpret_cast<char*>(&bitsPerPixel), 2);
    writer.write(reinterpret_cast<char*>(&compression), 4);
    writer.write(reinterpret_cast<char*>(&dataSize), 4);
    uint32_t printRes = 2835; // 72 DPI
    writer.write(reinterpret_cast<char*>(&printRes), 4);
    writer.write(reinterpret_cast<char*>(&printRes), 4);
    uint32_t paletteColors = 0;
    writer.write(reinterpret_cast<char*>(&paletteColors), 4);
    uint32_t importantColors = 0;
    writer.write(reinterpret_cast<char*>(&importantColors), 4);

    // --- Pixel Data ---
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const float3& col = image[y * width + x];
            uint8_t r = static_cast<uint8_t>(std::clamp(col.r() * 255.0f, 0.0f, 255.0f));
            uint8_t g = static_cast<uint8_t>(std::clamp(col.g() * 255.0f, 0.0f, 255.0f));
            uint8_t b = static_cast<uint8_t>(std::clamp(col.b() * 255.0f, 0.0f, 255.0f));
            uint8_t padding = 0;
            
            if (!writer.write(reinterpret_cast<char*>(&b), 1) ||
                !writer.write(reinterpret_cast<char*>(&g), 1) ||
                !writer.write(reinterpret_cast<char*>(&r), 1) ||
                !writer.write(reinterpret_cast<char*>(&padding), 1)) {
                throw std::runtime_error("Failed to write pixel data");
            }    
        }
    }

    writer.close();
    if (writer.fail()) {
        throw std::runtime_error("Failed to properly close the file");
    }
}

void render(const std::vector<float2>& points, const std::vector<float3>& triangle_colors, std::vector<float3>& image, int width, int height, int point_count){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            for(int i = 0; i < point_count; i += 3){
                float2 a(points[i + 0]);
                float2 b(points[i + 1]);
                float2 c(points[i + 2]);
                float2 p(x, y);

                if(Math::point_in_triangle(a, b, c, p)){
                    image[y * width + x] = triangle_colors[i / 3];
                }
            }
        }
    }
}

void create_test_images() {
    const int width = 256;
    const int height = 256;
    const int triangle_count = 256;
    
    float2 points[triangle_count * 3];
    float2 velocities[triangle_count * 3];
    float3 triangle_colors[triangle_count];

    float2 half_size(width / 2.0f, height / 2.0f);
    
    for (int i = 0; i < triangle_count * 3; i++){
        float2 p(Math::random_float2(width, height));
        points[i] = p;
        points[i].x = (points[i].x - half_size.x) * 0.3f + half_size.x;
        points[i].y = (points[i].y - half_size.y) * 0.3f + half_size.y;
    }

    for (int i = 0; i < triangle_count * 3; i += 3){

        float2 velocity(Math::random_float2(width, height));
        velocity.x = (velocity.x - half_size.x) * 0.5f;
        velocity.y = (velocity.y - half_size.y) * 0.5f;
        velocities[i + 0] = velocity;
        velocities[i + 1] = velocity;
        velocities[i + 2] = velocity;
        triangle_colors[i / 3] = Math::random_color();
    }
    
    std::vector<float2> points_vec(points, points + triangle_count * 3);
    std::vector<float3> triangle_colors_vec(triangle_colors, triangle_colors + triangle_count);
    std::vector<float3> image(width * height, float3(0, 0, 0));

    render(points_vec, triangle_colors_vec, image, width, height, triangle_count * 3);
    write_image_to_file(image, width, height, "output");

}



int main() {
    try {
        create_test_images();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

//za84rr1sje@vwhins.com