#include "Rasterizer.h"
#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include "Maths.h" 

void Rasterizer::Render(Scene& scene, RenderTarget& target) {
    // Simple clears
    std::fill(target.color_buffer.begin(), target.color_buffer.end(), float3(0.0f, 0.0f, 0.0f));
    std::fill(target.depth_buffer.begin(), target.depth_buffer.end(), 1000.0f);

    const int w = target.Width;
    const int h = target.Height;

    // Determine optimal thread count (leave one core for system)
    const unsigned int num_threads = std::max(1u, std::thread::hardware_concurrency() - 1);
    
    // Collect all triangles from all objects first
    struct TriangleData {
        float3 screen_vertices[3];
        float2 texture_coords[3];
        float3 normals[3];
        std::shared_ptr<ObjectShader> shader;
    };
    
    std::vector<TriangleData> all_triangles;
    
    for (Object& object : scene.objects) {
        ObjectMesh& model = object.Mesh;

        // Transform all vertices once
        if(scene.camera.CamTransform.has_changed || object.Obj_Transform.has_changed) { 
            object.process_object(target.Size, scene.camera); // Only update, when changed
            object.Obj_Transform.has_changed = false;
        }
        
        // Collect triangles from this object
        for (size_t i = 0; i + 2 < model.Vertices.size(); i += 3) {
            const float3 a3 = model.screen_vertices[i + 0];
            const float3 b3 = model.screen_vertices[i + 1];
            const float3 c3 = model.screen_vertices[i + 2];

            // Near-plane reject
            if (a3.z <= 0.1f || b3.z <= 0.1f || c3.z <= 0.1f) continue;

            TriangleData tri;
            tri.screen_vertices[0] = a3;
            tri.screen_vertices[1] = b3;
            tri.screen_vertices[2] = c3;
            
            tri.texture_coords[0] = model.Texture_cords[i + 0];
            tri.texture_coords[1] = model.Texture_cords[i + 1];
            tri.texture_coords[2] = model.Texture_cords[i + 2];
            
           
            tri.normals[0] = model.Normals[i + 0];
            tri.normals[1] = model.Normals[i + 1];
            tri.normals[2] = model.Normals[i + 2];
            
            tri.shader = object.Object_Shader;
            
            all_triangles.push_back(tri);
        }
    }
    
    // Thread-safe triangle counter
    std::atomic<size_t> triangle_index(0);
    
    // Lambda function for triangle rasterization worker
    auto rasterize_triangles = [&]() {
        size_t tri_idx;
        while ((tri_idx = triangle_index.fetch_add(1)) < all_triangles.size()) {
            const TriangleData& tri = all_triangles[tri_idx];
            
            const float3& a3 = tri.screen_vertices[0];
            const float3& b3 = tri.screen_vertices[1];
            const float3& c3 = tri.screen_vertices[2];

            const float2 a2(a3.x, a3.y);
            const float2 b2(b3.x, b3.y);
            const float2 c2(c3.x, c3.y);

            // Back-face cull via signed area (edge function equals twice-area)
            float area = Math::edge_function(a2, b2, c2);
            if (area > 0.0f) continue;   // Backface cull
            float invArea = 1.0f / area;

            // Triangle bounds (clamped to screen)
            const float min_xf = std::min({ a2.x, b2.x, c2.x });
            const float min_yf = std::min({ a2.y, b2.y, c2.y });
            const float max_xf = std::max({ a2.x, b2.x, c2.x });
            const float max_yf = std::max({ a2.y, b2.y, c2.y });

            if (max_xf < 0.0f || max_yf < 0.0f || min_xf >= w || min_yf >= h) continue;

            const int minX = std::max(0, (int)std::floor(min_xf));
            const int minY = std::max(0, (int)std::floor(min_yf));
            const int maxX = std::min(w - 1, (int)std::ceil(max_xf));
            const int maxY = std::min(h - 1, (int)std::ceil(max_yf));

            // Perspective-correct setup: pre-divide attributes by depth
            const float invZa = 1.0f / a3.z;
            const float invZb = 1.0f / b3.z;
            const float invZc = 1.0f / c3.z;

            const float u0 = tri.texture_coords[0].x;
            const float v0 = tri.texture_coords[0].y;
            const float u1 = tri.texture_coords[1].x;
            const float v1 = tri.texture_coords[1].y;
            const float u2 = tri.texture_coords[2].x;
            const float v2 = tri.texture_coords[2].y;

            const float u0z = u0 * invZa, v0z = v0 * invZa;
            const float u1z = u1 * invZb, v1z = v1 * invZb;
            const float u2z = u2 * invZc, v2z = v2 * invZc;

            // Normal interpolation (barycentric in screen space)
            float3 n0 = tri.normals[0];
            float3 n1 = tri.normals[1];
            float3 n2 = tri.normals[2];

            // Edge-function increments across x/y:
            const float dx_w0 = (c2.y - b2.y);
            const float dy_w0 = -(c2.x - b2.x);
            const float dx_w1 = (a2.y - c2.y);
            const float dy_w1 = -(a2.x - c2.x);
            const float dx_w2 = (b2.y - a2.y);
            const float dy_w2 = -(b2.x - a2.x);

            // Start at pixel center of the first row
            const float startX = (float)minX + 0.5f;
            float py = (float)minY + 0.5f;

            // Evaluate edge functions at row start
            float w0_row = Math::edge_function(b2, c2, float2(startX, py));
            float w1_row = Math::edge_function(c2, a2, float2(startX, py));
            float w2_row = Math::edge_function(a2, b2, float2(startX, py));

            for (int y = minY; y <= maxY; ++y) {
                float w0 = w0_row;
                float w1 = w1_row;
                float w2 = w2_row;

                const int rowOffset = y * w;

                for (int x = minX; x <= maxX; ++x) {
                    // Inside-test for CCW or CW
                    if ((w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
                        (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f))
                    {
                        float alpha = w0 * invArea;
                        float beta  = w1 * invArea;
                        float gamma = w2 * invArea;

                        const float depth = a3.z * alpha + b3.z * beta + c3.z * gamma;
                        const int idx = rowOffset + x;

                        if (depth < target.depth_buffer[idx]) {
                            const float iz = invZa * alpha + invZb * beta + invZc * gamma;
                            const float u = (u0z * alpha + u1z * beta + u2z * gamma) / iz;
                            const float v = (v0z * alpha + v1z * beta + v2z * gamma) / iz;
                            
                            // Color write
                            switch (tri.shader->type) {
                                case ShaderType::Texture: {
                                    auto* shader = static_cast<TextureShader*>(tri.shader.get());
                                    target.color_buffer[idx] = shader->Shader_texture->Sample(u, v);
                                    break;
                                }
                                case ShaderType::LitTexture: {
                                    auto* shader = static_cast<LitTextureShader*>(tri.shader.get());
                                    float3 normal(n0 * alpha + n1 * beta + n2 * gamma);
                                    float light_intensity = (Math::dot(normal, shader->Direction_to_light) + 1) * 0.5f;
                                    light_intensity = Math::lerp(0.2f, 1.0f, light_intensity);
                                    target.color_buffer[idx] = shader->Shader_texture->Sample(u, v) * light_intensity;
                                    break;
                                }
                            }
                            target.depth_buffer[idx] = depth;
                        }
                    }

                    // Move to next pixel
                    w0 += dx_w0;
                    w1 += dx_w1;
                    w2 += dx_w2;
                }

                // Move to next row
                w0_row += dy_w0;
                w1_row += dy_w1;
                w2_row += dy_w2;
            }
        }
    };

    static ThreadPool pool(num_threads); // Persistent across frames

    for (unsigned int i = 0; i < num_threads; ++i) {
        pool.enqueue(rasterize_triangles);
    }

    pool.wait(); // wait for all work to finish
    
    scene.camera.CamTransform.has_changed = false;
}