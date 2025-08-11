#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "Maths.h"
#include "Models.cpp"
#include "StringHelper.cpp"
#include "RenderTargets.cpp"
#include <filesystem>

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

void render(Model model, RenderTarget target){
    for(int i = 0; i < model.Points.size(); i += 3){
        float2 a(Math::world_to_screen(model.Points[i + 0], target.Size));
        float2 b(Math::world_to_screen(model.Points[i + 1], target.Size));
        float2 c(Math::world_to_screen(model.Points[i + 2], target.Size));

        // Triangle bounds
        float min_x = std::min(std::min(a.x, b.x), c.x);
        float min_y = std::min(std::min(a.y, b.y), c.y);
        float max_x = std::max(std::max(a.x, b.x), c.x);
        float max_y = std::max(std::max(a.y, b.y), c.y);

        // Pixel block covering the triangle bounds
        int block_start_x = std::clamp((int)min_x, 0, target.Width - 1);
        int block_start_y = std::clamp((int)min_y, 0, target.Height -1);
        int block_end_x = std::clamp((int)ceil(max_x), 0, target.Width -1);
        int block_end_y = std::clamp((int)ceil(max_y), 0, target.Height - 1);

        // Loop over the block
        for(int y = block_start_y; y <= block_end_y; y++){
            for(int x = block_start_x; x <= block_end_x; x++){
                if(!Math::point_in_triangle(a, b, c, float2(x, y))) continue;
                target.color_buffer[y * target.Width + x] = model.Triangle_colors[i / 3];
            }
        }
    }
}

void create_test_images(int width, int height) {
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
    
    for (int frame = 0; frame < 60; frame++) {
        // Convert arrays to vectors for render
        std::vector<float2> points_vec(points, points + triangle_count * 3);
        std::vector<float3> triangle_colors_vec(triangle_colors, triangle_colors + triangle_count);
        std::vector<float3> image(width * height, float3(0, 0, 0)); // clear to black

        // Render
        //TODO either remove this or rememnber to delete the comment
        //render(points_vec, triangle_colors_vec, image, width, height, triangle_count * 3);

        // Save image
        std::string filename = "frame_" + std::to_string(frame);
        write_image_to_file(image, width, height, filename);

        // Update points positions
        for (int i = 0; i < triangle_count * 3; i++) {
            points[i].x += velocities[i].x * 0.1f; // speed scale
            points[i].y += velocities[i].y * 0.1f;

            // Bounce off edges
            if (points[i].x < 0 || points[i].x >= width) {
                velocities[i].x *= -1;
            }
            if (points[i].y < 0 || points[i].y >= height) {
                velocities[i].y *= -1;
            }
        }
    }

}



int main() {
    int width = 256;
    int height = 256;

    // Load cube data
    std::string obj_path = std::filesystem::current_path().string() + "/Objects/Cube.obj";
    std::string obj_string = StringHelper::readFileToString(obj_path);
    std::vector<float3> cube_model_points = ObjLoader::load_obj_file(obj_string);

    std::vector<float3> triangle_colors;
    for(int i = 0; i < cube_model_points.size() / 3; i++){
        triangle_colors[i] = Math::random_color();
    }


    Model cube_model(cube_model_points, triangle_colors);
    RenderTarget render_target(width, height);

    render(cube_model, render_target);
    write_image_to_file(render_target.color_buffer, render_target.Width, render_target.Height, "Test");

    return 0;
}
//za84rr1sje@vwhins.com