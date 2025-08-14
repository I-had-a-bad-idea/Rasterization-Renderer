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

float3 float3::operator+(const float3& other){
    return float3(x + other.x, y + other.y, z + other.z);
}

float3 float3::operator-(const float3& other){
    return float3(x - other.x, y - other.y, z - other.z);
}

float3 float3::operator*(float scalar){
    return float3(x * scalar, y * scalar, z * scalar);
}

float3 float3::operator/(float scalar) const {
    return float3(x / scalar, y / scalar, z / scalar);
}

float3& float3::operator+=(const float3& other) {
    x += other.x; y += other.y; z += other.z;
    return *this;
}

float3& float3::operator-=(const float3& other) {
    x -= other.x; y -= other.y; z -= other.z;
    return *this;
}

float3& float3::operator*=(float scalar) {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
}

float3& float3::operator/=(float scalar) {
    x /= scalar; y /= scalar; z /= scalar;
    return *this;
}

float float3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

float3 float3::Normalize(const float3& v) {
    float len = v.length();
    if (len > 0.0f)
        return v / len;
    return float3(0.0f, 0.0f, 0.0f);
}