#include <algorithm>
#include <cmath>
#include "Maths.h"
#include "Vector.h"
#include "ObjectTransform.h"

float Math::random_float(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
}

float2 Math::random_float2(float width, float height){
    return float2(random_float(0, width), random_float(0,height));
}

float3 Math::random_color(){
    return float3(random_float(0.0, 1.0), random_float(0.0, 1.0), random_float(0.0, 1.0));
}

// Calculate the dot product of a and b
float Math::dot(float2 a, float2 b){
    return a.x * b.x + a.y * b.y;
}

// Rotate vec by 90 degrees (clockwise)
float2 Math::perpendicular(float2 vec){
    return float2(vec.y, -vec.x);
} 

// Check if point p is on the right side of the line a -> b
bool Math::point_on_right_side_of_line(float2 a, float2 b, float2 p){
    float2 ap(p.x - a.x, p.y - a.y);
    float2 ab_perp(perpendicular(float2(b.x - a.x, b.y - a.y)));

    return dot(ap, ab_perp) > 0.0f;
}

// Test if point p is inside triangle abc
bool Math::point_in_triangle(float2 a, float2 b, float2 c, float2 p){
//    // Calculate barycentric coordinates
//    float2 v0 = {c.x - a.x, c.y - a.y};
//    float2 v1 = {b.x - a.x, b.y - a.y};
//    float2 v2 = {p.x - a.x, p.y - a.y};
//
//    float dot00 = dot(v0, v0);
//    float dot01 = dot(v0, v1);
//   float dot02 = dot(v0, v2);
//    float dot11 = dot(v1, v1);
//    float dot12 = dot(v1, v2);
//
//    // Compute barycentric coordinates
//    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
//    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
//    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
//
//    // Check if point is in triangle
//    return (u >= 0) && (v >= 0) && (u + v <= 1);

    bool side_ab = point_on_right_side_of_line(a, b, p);
    bool side_bc = point_on_right_side_of_line(b, c, p);
    bool side_ca = point_on_right_side_of_line(c, a, p);

    return side_ab && side_bc && side_ca;


}

// Converts world-space coordinates into screen-space coordinate
float2 Math::world_to_screen(float3 vertex, ObjectTransform transform, float2 numPixels) {
    // Scale world coordinates to fit nicely in the screen
    float3 vertex_world;
    vertex_world = transform.ToWorldPoint(vertex);

    float screen_height_world = 5;
    float pixels_per_world_unit = numPixels.y / screen_height_world / vertex_world.z;
    
    float2 pixel_offset(vertex_world.x * pixels_per_world_unit,
                        vertex_world.y * pixels_per_world_unit);

    return float2(numPixels.x / 2 + pixel_offset.x,
                numPixels.y / 2 + pixel_offset.y);
}