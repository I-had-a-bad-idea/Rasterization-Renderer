#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>

class float2 {
public:
    float x, y;

    float2(float x = 0.0f, float y = 0.0f);
    float length();
};

class float3 {
public:
    float x, y, z;

    float3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    float r() const;
    float g() const;
    float b() const;
    void set_r(float value);
    void set_g(float value);
    void set_b(float value);
};

#endif