#include "Transform.h"
#include <cmath>


Transform::Transform() : Yaw(60.0f) {} 
Transform::Transform(float yaw) : Yaw(yaw) {}

float3 Transform::ToWorldPoint(float3 p)
{
    auto [ihat, jhat, khat] = GetBasisVectors();
    return TransformVector(ihat, jhat, khat, p);
}

// Calculate right/up/forward vectors (i, j, k)
std::tuple<float3, float3, float3> Transform::GetBasisVectors()
{
    float3 ihat(std::cos(Yaw), 0, std::sin(Yaw));
    float3 jhat(0, 1, 0);
    float3 khat(-std::sin(Yaw), 0, std::cos(Yaw));
    return std::make_tuple(ihat, jhat, khat);
}

// Move each coordinate of given vector along the corresponding basis vector
float3 Transform::TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v)
{
    return float3(
        v.x * ihat.x + v.y * jhat.x + v.z * khat.x,
        v.x * ihat.y + v.y * jhat.y + v.z * khat.y,
        v.x * ihat.z + v.y * jhat.z + v.z * khat.z
    );
}