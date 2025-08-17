#include "ObjectTransform.h"
#include <cmath>


ObjectTransform::ObjectTransform() : Position(float3(0, 0, 0)), Rotation(float3(0, 0, 0)) {} 
ObjectTransform::ObjectTransform(float3 position, float3 rotation) : Position(position), Rotation(rotation) {}

float3 ObjectTransform::ToWorldPoint(float3 p)
{
    auto [ihat, jhat, khat] = GetBasisVectors();
    return TransformVector(ihat, jhat, khat, p) + Position;
}

float3 ObjectTransform::ToLocalPoint(float3 world_point) {
    auto [ihat, jhat, khat] = GetInverseBasisVectors();
    float3 relative = world_point - Position;
    return TransformVector(ihat, jhat, khat, relative);
}

// Calculate right/up/forward vectors (i, j, k)
std::tuple<float3, float3, float3> ObjectTransform::GetBasisVectors()
{
    float3 ihat_yaw(std::cos(Rotation.y), 0, std::sin(Rotation.y));
    float3 jhat_yaw(0, 1, 0);
    float3 khat_yaw(-std::sin(Rotation.y), 0, std::cos(Rotation.y));

    float3 ihat_pitch(1, 0, 0);
    float3 jhat_pitch(0, std::cos(Rotation.x), -std::sin(Rotation.x));
    float3 khat_pitch(0, std::sin(Rotation.x), std::cos(Rotation.x));

    float3 ihat_roll(std::cos(Rotation.z), std::sin(Rotation.z), 0);
    float3 jhat_roll(-std::sin(Rotation.z), std::cos(Rotation.z), 0);
    float3 khat_roll(0, 0, 1);

    // ---- Yaw and Pitch combined ----
    float3 ihat_pitchYaw = TransformVector(ihat_yaw, jhat_yaw, khat_yaw, ihat_pitch);
    float3 jhat_pitchYaw = TransformVector(ihat_yaw, jhat_yaw, khat_yaw, jhat_pitch);
    float3 khat_pitchYaw = TransformVector(ihat_yaw, jhat_yaw, khat_yaw, khat_pitch);
    // Combine roll
    float3 ihat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, ihat_roll);
    float3 jhat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, jhat_roll);
    float3 khat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, khat_roll);
    return std::make_tuple(ihat, jhat, khat);
}

std::tuple<float3, float3, float3> ObjectTransform::GetInverseBasisVectors(){
    auto[ihat_, jhat_, khat_] = GetBasisVectors();
    float3 ihat(ihat_.x, jhat_.x, khat_.x);
    float3 jhat(ihat_.y, jhat_.y, khat_.y);
    float3 khat(ihat_.z, jhat_.z, khat_.z);


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