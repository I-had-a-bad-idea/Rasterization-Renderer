#ifndef MATH_H
#define MATH_H

#include "Camera.h"
#include "Vector.h"
#include "ObjectTransform.h"
#include <numbers>
#include <algorithm>
#include <cmath>


class Math {
public:
    static float random_float(float min, float max);
    static float2 random_float2(float width, float height);
    static float3 random_color();
    static float dot(float2 a, float2 b);
    static float dot(float3 a, float3 b);
    static float2 perpendicular(float2 vec);
    static bool point_on_right_side_of_line(float2 a, float2 b, float2 p);
    static bool point_in_triangle(float2 a, float2 b, float2 c, float2 p, float3& weigths);
    static float3 world_to_screen(float3 vertex, ObjectTransform transform, float2 target_size, Cam camera);
    static float signed_triangle_area(float2 a, float2 b, float2 c);
    static float degrees_to_radians(float degrees);
    static float radians_to_degrees(float radians);
    static float lerp(float a, float b, float c);
};

#endif