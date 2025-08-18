#include "Rasterizer.h"


void Rasterizer::Render(Scene& scene, RenderTarget& target) {
    // Clear the color buffer first
    std::fill(target.color_buffer.begin(), target.color_buffer.end(), float3(0.0f, 0.0f, 0.0f)); // Black background
    // Reset depth buffer with large depth values
    std::fill(target.depth_buffer.begin(), target.depth_buffer.end(), 1000.0f);
    
    // Loop over each object in the scene
    for (Object& object : scene.objects) {
        ObjectMesh& model = object.Mesh;
        object.process_object(target.Size, scene.camera);
        for (size_t i = 0; i + 2 < model.Vertices.size(); i += 3) {
            float3 a = model.screen_vertices[i + 0];
            float3 b = model.screen_vertices[i + 1];
            float3 c = model.screen_vertices[i + 2];

            // Skip triangles that are behind the camera
            if (a.z <= 0.01f || b.z <= 0.01f || c.z <= 0.01f) continue;

            // **Backface culling** - calculate avarage normal of the triangle
            float3 normal = (a + b + c) / 3;
            float3 cameraToTriangle = float3(a.x - scene.camera.CamTransform.Position.x, a.y - scene.camera.CamTransform.Position.y, a.z - scene.camera.CamTransform.Position.z);
            
            // If the normal is facing away from the camera , cull the triangle
            if (Math::dot(normal, cameraToTriangle) < 0) continue;

            // Ensure we have a valid triangle color index
            int color_index = (i / 3) % object.Triangle_colors.size();

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
                        if (depth > target.depth_buffer[y * target.Width + x]) {
                            continue;  // Skip pixel as it is farther away
                            }
                        // Perspective-correct interpolation
                        float u0 = model.Texture_cords[i+0].x;
                        float v0 = model.Texture_cords[i+0].y;
                        float u1 = model.Texture_cords[i+1].x;
                        float v1 = model.Texture_cords[i+1].y;
                        float u2 = model.Texture_cords[i+2].x;
                        float v2 = model.Texture_cords[i+2].y;

                        // Pre-divide by depth
                        float u0z = u0 / a.z;
                        float v0z = v0 / a.z;
                        float u1z = u1 / b.z;
                        float v1z = v1 / b.z;
                        float u2z = u2 / c.z;
                        float v2z = v2 / c.z;

                        float w0 = 1.0f / a.z;
                        float w1 = 1.0f / b.z;
                        float w2 = 1.0f / c.z;

                        // Interpolate
                        float u = u0z * weights.x + u1z * weights.y + u2z * weights.z;
                        float v = v0z * weights.x + v1z * weights.y + v2z * weights.z;
                        float w = w0  * weights.x + w1  * weights.y + w2  * weights.z;

                        // Undo division
                        u /= w;
                        v /= w;

                        float2 text_coord(u, v);
                        float3 normal = model.Normals[i + 0] * weights.x + model.Normals[i + 1] * weights.y + model.Normals[i + 2] * weights.z;
                        normal = float3::Normalize(normal);

                        target.color_buffer[y * target.Width + x] = object.Object_Shader->PixelColor(text_coord, normal);
                        target.depth_buffer[y * target.Width + x] = depth;
                        
                    }
                }
            }
        }
    }
}