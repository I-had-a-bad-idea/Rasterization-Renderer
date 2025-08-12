#include "Vector.h"

float2::float2(float x, float y) : x(x), y(y) {}

float float2::length() {
    return std::sqrt(x * x + y * y);
}

float3::float3(float x, float y, float z) : x(x), y(y), z(z) {}

float float3::r() { return x; }
float float3::g() { return y; }
float float3::b() { return z; }

void float3::set_r(float value) { x = std::clamp(value, 0.0f, 1.0f); }
void float3::set_g(float value) { y = std::clamp(value, 0.0f, 1.0f); }
void float3::set_b(float value) { z = std::clamp(value, 0.0f, 1.0f); }