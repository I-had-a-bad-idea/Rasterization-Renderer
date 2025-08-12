#include <algorithm>
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

float Math::dot(float3 a, float3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
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
// Non-clockwise triangles are considerd "back-faces" and are ignored
bool Math::point_in_triangle(float2 a, float2 b, float2 c, float2 p, float3& weigths){
    
    float areaABP = signed_triangle_area(a, b, p);
    float areaBCP = signed_triangle_area(b, c, p);
    float areaCAP = signed_triangle_area(c, a, p);
    
    bool in_tri = areaABP >= 0 && areaBCP >= 0 && areaCAP >= 0;

    // Weigthing factors (barycentric coordinates)
    float total_area = areaABP + areaBCP + areaCAP;
    float inv_area_sum = 1 / total_area;
    float weigthA = areaBCP * inv_area_sum;
    float weightB = areaCAP * inv_area_sum;
    float weightC = areaABP * inv_area_sum;

    weigths = float3(weigthA, weightB, weightC);


    return in_tri && total_area > 0;


}

// Converts world-space coordinates into screen-space coordinate
float3 Math::world_to_screen(float3 vertex, ObjectTransform transform, float2 numPixels, float fov) {
    // Scale world coordinates to fit nicely in the screen
    float3 vertex_world;
    vertex_world = transform.ToWorldPoint(vertex);
    float screen_height_world = std::tan(fov / 2.0f) * 2.0f;
    float pixels_per_world_unit = numPixels.y / screen_height_world / vertex_world.z;
    
    float2 pixel_offset(vertex_world.x * pixels_per_world_unit,
                        vertex_world.y * pixels_per_world_unit);
    
    float2 vertex_screen(numPixels.x / 2 + pixel_offset.x,
                numPixels.y / 2 + pixel_offset.y); 

    return float3(vertex_screen.x, vertex_screen.y, vertex_world.z);
}

float Math::signed_triangle_area(float2 a, float2 b, float2 c){
    float2 ac(c.x -a.x, c.y - a.y);
    float2 abperp(perpendicular(float2(b.x - a.x, b.y - a.y)));
    
    return dot(ac, abperp) / 2; 
}
