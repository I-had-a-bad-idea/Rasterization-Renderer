#include "Maths.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Generate a random float
float Math::random_float(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
}

// Generate a random float2
float2 Math::random_float2(float width, float height){
    return float2(random_float(0, width), random_float(0,height));
}

// Generate a random float3 (color)
float3 Math::random_color(){
    return float3(random_float(0.0, 1.0), random_float(0.0, 1.0), random_float(0.0, 1.0));
}

// Calculate the dot product of a and b
float Math::dot(float2 a, float2 b){
    return a.x * b.x + a.y * b.y;
}
// Calculate the dot product of aand b // Same as above just 3D
float Math::dot(float3 a, float3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Rotate vec by 90 degrees (clockwise)
float2 Math::perpendicular(float2 vec){
    return float2(vec.y, -vec.x);
} 

float Math::degrees_to_radians(float degrees) {
    return degrees * M_PI / 180.0f;
}

float Math::radians_to_degrees(float radians) {
    return radians * 180.0f / M_PI;
}

// Check if point p is on the right side of the line a -> b
bool Math::point_on_right_side_of_line(float2 a, float2 b, float2 p){
    float2 ap(p.x - a.x, p.y - a.y);
    float2 ab_perp(perpendicular(float2(b.x - a.x, b.y - a.y)));

    return dot(ap, ab_perp) > 0.0f;
}

// Test if point p is inside triangle abc using barycentric coordinates
// This also handles backface culling by checking triangle orientation
bool Math::point_in_triangle(float2 a, float2 b, float2 c, float2 p, float3& weights){
    
    // Calculate barycentric coordinates
    float2 v0 = float2(c.x - a.x, c.y - a.y);
    float2 v1 = float2(b.x - a.x, b.y - a.y);
    float2 v2 = float2(p.x - a.x, p.y - a.y);
    
    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);
    
    // Calculate barycentric coordinates
    float inv_denom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
    
    // Check if point is in triangle
    bool in_triangle = (u >= 0) && (v >= 0) && (u + v <= 1);
    
    if (in_triangle) {
        // Calculate weights for interpolation (barycentric coordinates)
        weights.x = 1.0f - u - v;  // weight for vertex a
        weights.y = v;              // weight for vertex b  
        weights.z = u;              // weight for vertex c
        
        // Check triangle orientation for backface culling
        // Calculate cross product to determine winding order
        float cross = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
        
        // Only render counter-clockwise triangles (front faces)
        return cross > 0;
    }
    
    return false;
}


// Converts world-space coordinates into screen-space coordinate
float3 Math::world_to_screen(float3 vertex, ObjectTransform transform, float2 numPixels, Cam camera) {
    // Transform vertex to world space
    float3 vertex_world = transform.ToWorldPoint(vertex);
    float3 vertex_view = camera.CamTransform.ToLocalPoint(vertex_world);
    
    // Convert FOV from degrees to radians
    float fov_radians = degrees_to_radians(camera.Fov);
    
    // Perspective projection - simpler and more reliable approach
    float z = vertex_view.z;
    if (z <= 0.01f) z = 0.01f; // Prevent division by zero and handle near clipping
    
    // Calculate projection scale
    float scale = 1.0f / std::tan(fov_radians * 0.5f);
    float aspect_ratio = numPixels.x / numPixels.y;
    
    // Project to normalized device coordinates
    float ndc_x = (vertex_view.x * scale) / (z * aspect_ratio);
    float ndc_y = (vertex_view.y * scale) / z;
    
    // Convert to screen coordinates (0,0 at top-left)
    float screen_x = (ndc_x + 1.0f) * 0.5f * numPixels.x;
    float screen_y = (1.0f - ndc_y) * 0.5f * numPixels.y; // Flip Y for screen coordinates
    
    return float3(screen_x, screen_y, z);
}

float Math::signed_triangle_area(float2 a, float2 b, float2 c){
    return 0.5f * ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
}

float Math::lerp(float a, float b, float c){
    return a + (b - a) * std::clamp(c, 0.0f, 1.0f);
}