#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>

class float2 {
public:
    float x, y;

    float2(float x = 0.0f, float y = 0.0f);
    float length();


    float2 operator+(const float2& other) const;
    float2 operator-(const float2& other) const;
    float2 operator*(float scalar) const;
    float2 operator/(float scalar) const;

    float2& operator+=(const float2& other);
    float2& operator-=(const float2& other);
    float2& operator*=(float scalar);
    float2& operator/=(float scalar);
};

class float3 {
public:
    float x, y, z;

    float3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    float r();
    float g();
    float b();
    void set_r(float value);
    void set_g(float value);
    void set_b(float value);

    float3 operator+(const float3& other);
    float3 operator-(const float3& other);
    float3 operator*(float scalar);
    float3 operator/(float scalar) const;

    float3& operator+=(const float3& other);
    float3& operator-=(const float3& other);
    float3& operator*=(float scalar);
    float3& operator/=(float scalar);

    float length() const;
    static float3 Normalize(const float3& v);
    static float3 cross(const float3& a, const float3& b);
};

#endif