#include "ObjectTransform.h"
#include <cmath>


ObjectTransform::ObjectTransform() : Position(float3(0, 0, 0)), Yaw(0.0f), Pitch(0.0f) {} 
ObjectTransform::ObjectTransform(float3 position, float yaw, float pitch) : Position(position), Yaw(yaw), Pitch(pitch) {}

float3 ObjectTransform::ToWorldPoint(float3 p)
{
    auto [ihat, jhat, khat] = GetBasisVectors();
    return TransformVector(ihat, jhat, khat, p) + Position;
}

float3 ObjectTransform::ToLocalPoint(float3 world_point) {
    auto [ihat, jhat, khat] = GetInverseBasisVectors();
    return TransformVector(ihat, jhat, khat, world_point) - Position;
}

// Calculate right/up/forward vectors (i, j, k)
std::tuple<float3, float3, float3> ObjectTransform::GetBasisVectors()
{
    float3 ihat_yaw(std::cos(Yaw), 0, std::sin(Yaw));
    float3 jhat_yaw(0, 1, 0);
    float3 khat_yaw(-std::sin(Yaw), 0, std::cos(Yaw));

    float3 ihat_pitch(1, 0, 0);
    float3 jhat_pitch(0, std::cos(Pitch), -std::sin(Pitch));
    float3 khat_pitch(0, std::sin(Pitch), std::cos(Pitch));

    float3 ihat(TransformVector(ihat_yaw, jhat_yaw, khat_yaw, ihat_pitch));
    float3 jhat(TransformVector(ihat_yaw, jhat_yaw, khat_yaw, jhat_pitch));
    float3 khat(TransformVector(ihat_yaw, jhat_yaw, khat_yaw, khat_pitch));

    return std::make_tuple(ihat, jhat, khat);
}

std::tuple<float3, float3, float3> ObjectTransform::GetInverseBasisVectors(){
    float3 ihat_yaw(std::cos(-Yaw), 0, std::sin(-Yaw));
    float3 jhat_yaw(0, 1, 0);
    float3 khat_yaw(-std::sin(-Yaw), 0, std::cos(-Yaw));

    float3 ihat_pitch(1, 0, 0);
    float3 jhat_pitch(0, std::cos(-Pitch), -std::sin(-Pitch));
    float3 khat_pitch(0, std::sin(-Pitch), std::cos(-Pitch));

    float3 ihat(TransformVector(ihat_pitch, jhat_pitch, khat_pitch, ihat_yaw));
    float3 jhat(TransformVector(ihat_pitch, jhat_pitch, khat_pitch, jhat_yaw));
    float3 khat(TransformVector(ihat_pitch, jhat_pitch, khat_pitch, khat_yaw));

    return std::make_tuple(ihat, jhat, khat);
}

// Move each coordinate of given vector along the corresponding basis vector
float3 ObjectTransform::TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v)
{
    return float3(
        v.x * ihat.x + v.y * jhat.x + v.z * khat.x,
        v.x * ihat.y + v.y * jhat.y + v.z * khat.y,
        v.x * ihat.z + v.y * jhat.z + v.z * khat.z
    );
}