#include "Rasterizer.h"
#include <algorithm>
#include <cmath>
#include "Maths.h"  // for Math::edge_function

void Rasterizer::Render(Scene& scene, RenderTarget& target) {
    // Simple clears (predictable; swap to lazy-clears later if you want)
    std::fill(target.color_buffer.begin(), target.color_buffer.end(), float3(0.0f, 0.0f, 0.0f));
    std::fill(target.depth_buffer.begin(), target.depth_buffer.end(), 1000.0f);

    const int w = target.Width;
    const int h = target.Height;

    for (Object& object : scene.objects) {
        ObjectMesh& model = object.Mesh;

        // Transform all vertices once
        object.process_object(target.Size, scene.camera);

        // Triangles laid out as 0..N step 3
        for (size_t i = 0; i + 2 < model.Vertices.size(); i += 3) {
            const float3 a3 = model.screen_vertices[i + 0];
            const float3 b3 = model.screen_vertices[i + 1];
            const float3 c3 = model.screen_vertices[i + 2];

            // Near-plane reject
            if (a3.z <= 0.01f || b3.z <= 0.01f || c3.z <= 0.01f) continue;

            const float2 a2(a3.x, a3.y);
            const float2 b2(b3.x, b3.y);
            const float2 c2(c3.x, c3.y);

            // Back-face cull via signed area (edge function equals twice-area)
            float area = Math::edge_function(a2, b2, c2);
//            if (area == 0.0f) continue;   // keep CCW only
            float invArea = 1.0f / area;

            // Triangle bounds (clamped to screen)
            const float min_xf = std::min({ a2.x, b2.x, c2.x });
            const float min_yf = std::min({ a2.y, b2.y, c2.y });
            const float max_xf = std::max({ a2.x, b2.x, c2.x });
            const float max_yf = std::max({ a2.y, b2.y, c2.y });

            if (max_xf < 0.0f || max_yf < 0.0f || min_xf >= w || min_yf >= h) continue;

            const int minX = std::max(0, (int)std::floor(min_xf));
            const int minY = std::max(0, (int)std::floor(min_yf));
            const int maxX = std::min(w - 1, (int)std::ceil (max_xf));
            const int maxY = std::min(h - 1, (int)std::ceil (max_yf));

            // Perspective-correct setup: pre-divide attributes by depth
            const float invZa = 1.0f / a3.z;
            const float invZb = 1.0f / b3.z;
            const float invZc = 1.0f / c3.z;

            const float u0 = model.Texture_cords[i + 0].x;
            const float v0 = model.Texture_cords[i + 0].y;
            const float u1 = model.Texture_cords[i + 1].x;
            const float v1 = model.Texture_cords[i + 1].y;
            const float u2 = model.Texture_cords[i + 2].x;
            const float v2 = model.Texture_cords[i + 2].y;

            const float u0z = u0 * invZa, v0z = v0 * invZa;
            const float u1z = u1 * invZb, v1z = v1 * invZb;
            const float u2z = u2 * invZc, v2z = v2 * invZc;

            // Normal interpolation (barycentric in screen space)
            float3 n0 = model.Normals[i + 0];
            float3 n1 = model.Normals[i + 1];
            float3 n2 = model.Normals[i + 2];

            // Edge-function increments across x/y:
            // E_ab(p) = edge(a,b,p); dE/dx = (b.y-a.y); dE/dy = -(b.x-a.x)
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

                            float3 normal = float3::Normalize(n0 * alpha + n1 * beta + n2 * gamma);

                            target.color_buffer[idx] = object.Object_Shader->PixelColor(float2(u, v), normal);
                            target.depth_buffer[idx] = depth;
                        }
                    }

                    // ✅ Move to next pixel (must be inside x-loop)
                    w0 += dx_w0;
                    w1 += dx_w1;
                    w2 += dx_w2;
                }

                // ✅ Move to next row (outside x-loop, inside y-loop)
                w0_row += dy_w0;
                w1_row += dy_w1;
                w2_row += dy_w2;
            }
        }
    }
}