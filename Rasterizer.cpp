#include "Rasterizer.h"

void Rasterizer::write_image_to_file(std::vector<float3>& image, int width, int height, const std::string& name) {
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

void Rasterizer::Render(const std::vector<Object>& models, RenderTarget& target, float fov) {
    // Clear the color buffer first
    std::fill(target.color_buffer.begin(), target.color_buffer.end(), float3(0.0f, 0.0f, 0.0f)); // Black background
    // Reset depth buffer with large depth values
    std::fill(target.depth_buffer.begin(), target.depth_buffer.end(), 1000.0f);
    
    // Loop over each object in the scene
    for (const auto& model : models) {
        for (size_t i = 0; i + 2 < model.Points.size(); i += 3) {
            float3 a = Math::world_to_screen(model.Points[i + 0], model.Obj_Transform, target.Size, fov);
            float3 b = Math::world_to_screen(model.Points[i + 1], model.Obj_Transform, target.Size, fov);
            float3 c = Math::world_to_screen(model.Points[i + 2], model.Obj_Transform, target.Size, fov);

            // Skip triangles that are behind the camera
            if (a.z <= 0.1f || b.z <= 0.1f || c.z <= 0.1f) continue;

            // Ensure we have a valid triangle color index
            int color_index = (i / 3) % model.Triangle_colors.size();

            // Triangle bounds for scissor test
            float min_x = std::min({a.x, b.x, c.x});
            float min_y = std::min({a.y, b.y, c.y});
            float max_x = std::max({a.x, b.x, c.x});
            float max_y = std::max({a.y, b.y, c.y});

            // Skip triangles completely outside screen bounds
            if (max_x < 0 || min_x >= target.Width || max_y < 0 || min_y >= target.Height) continue;

            // Pixel block covering the triangle bounds
            int block_start_x = std::max(0, (int)std::floor(min_x));
            int block_start_y = std::max(0, (int)std::floor(min_y));
            int block_end_x = std::min(target.Width - 1, (int)std::ceil(max_x));
            int block_end_y = std::min(target.Height - 1, (int)std::ceil(max_y));

            // Loop over the block
            for (int y = block_start_y; y <= block_end_y; y++) {
                for (int x = block_start_x; x <= block_end_x; x++) {
                    float2 p(x + 0.5f, y + 0.5f);
                    float3 weights;

                    
                    if (Math::point_in_triangle(float2(a.x, a.y), float2(b.x, b.y), float2(c.x, c.y), p, weights)) {
                        // Interpolate depth using barycentric coordinates
                        float depth = a.z * weights.x + b.z * weights.y + c.z * weights.z;
                        
                        // Depth test: render if this pixel is closer
                        if (depth < target.depth_buffer[y * target.Width + x]) {
                            target.color_buffer[y * target.Width + x] = model.Triangle_colors[color_index];
                            target.depth_buffer[y * target.Width + x] = depth;
                        }
                    }
                }
            }
        }
    }
}