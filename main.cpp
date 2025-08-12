#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include "Vector.h"
#include "RenderTarget.h"
#include "Model.h"
#include "Maths.h"
#include "StringHelper.h"

void write_image_to_file(std::vector<float3>& image, int width, int height, const std::string& name) {
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

    // --- Pixel Data (BMP stores bottom-to-top) ---
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            float3 col = image[y * width + x];
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

void render(Model model, RenderTarget& target) {
    // Clear the color buffer first
    std::fill(target.color_buffer.begin(), target.color_buffer.end(), float3(0.0f, 0.0f, 0.0f)); // Blackbackground
    
    for(int i = 0; i < model.Points.size(); i += 3) {
        float2 a = Math::world_to_screen(model.Points[i + 0], model.Model_transform, target.Size);
        float2 b = Math::world_to_screen(model.Points[i + 1], model.Model_transform, target.Size);
        float2 c = Math::world_to_screen(model.Points[i + 2], model.Model_transform, target.Size);

        std::cout << "Triangle " << (i/3) << ": ";
        std::cout << "(" << model.Points[i + 0].x << ", " << model.Points[i + 0].y << ", " << model.Points[i + 0].z << ") -> (" << a.x << ", " << a.y << ") \t";
        std::cout << "(" << model.Points[i + 1].x << ", " << model.Points[i + 1].y << ", " << model.Points[i + 1].z << ") -> (" << b.x << ", " << b.y << ") \t";
        std::cout << "(" << model.Points[i + 2].x << ", " << model.Points[i + 2].y << ", " << model.Points[i + 2].z << ") -> (" << c.x << ", " << c.y << ")" << std::endl;

        // Triangle bounds
        float min_x = std::min({a.x, b.x, c.x});
        float min_y = std::min({a.y, b.y, c.y});
        float max_x = std::max({a.x, b.x, c.x});
        float max_y = std::max({a.y, b.y, c.y});

        // Pixel block covering the triangle bounds
        int block_start_x = std::max(0, (int)std::floor(min_x));
        int block_start_y = std::max(0, (int)std::floor(min_y));
        int block_end_x = std::min(target.Width - 1, (int)std::ceil(max_x));
        int block_end_y = std::min(target.Height - 1, (int)std::ceil(max_y));

        // Ensure we have a valid triangle color index
        int color_index = (i / 3) % model.Triangle_colors.size();

        // Loop over the block
        for(int y = block_start_y; y <= block_end_y; y++) {
            for(int x = block_start_x; x <= block_end_x; x++) {
                // Add 0.5 offset for pixel center sampling
                if(Math::point_in_triangle(a, b, c, float2(x + 0.5f, y + 0.5f))) {
                    target.color_buffer[y * target.Width + x] = model.Triangle_colors[color_index];
                }
            }
        }
    }
}


int main() {
    int width = 256 * 4;  // Increased resolution for better visibility
    int height = 256 * 4;

    // Load cube data
    std::string obj_path = std::filesystem::current_path().string() + "/Objects/Cube.obj";
    std::string obj_string = StringHelper::readFileToString(obj_path);
    std::vector<float3> cube_model_points = ObjLoader::load_obj_file(obj_string);

    if (cube_model_points.empty()) {
        std::cerr << "Failed to load cube model or model is empty!" << std::endl;
        return -1;
    }

    std::vector<float3> triangle_colors(cube_model_points.size() / 3);
    for(int i = 0; i < cube_model_points.size() / 3; i++){
        triangle_colors[i] = Math::random_color();
    }

    Model cube_model(cube_model_points, triangle_colors);
    RenderTarget render_target(width, height);
    
    std::cout << "Loaded points: " << cube_model_points.size() << std::endl;
    std::cout << "Number of triangles: " << cube_model_points.size() / 3 << std::endl;

    render(cube_model, render_target);
    write_image_to_file(render_target.color_buffer, render_target.Width, render_target.Height, "Cube-rotated");

    std::cout << "Rendering complete! Check Test.bmp" << std::endl;
    return 0;
}