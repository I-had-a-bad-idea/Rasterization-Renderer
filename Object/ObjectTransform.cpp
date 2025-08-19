#include "ObjectTransform.h"
#include <cmath>

ObjectTransform::ObjectTransform() 
    : Position(float3(0, 0, 0)), Rotation(float3(0, 0, 0)) 
{
    UpdateBasisVectors();
} 

ObjectTransform::ObjectTransform(float3 position, float3 rotation) 
    : Position(position), Rotation(rotation) 
{
    UpdateBasisVectors();
} 

void ObjectTransform::SetRotation(const float3& rot) {
    Rotation = rot;
    UpdateBasisVectors();
    has_changed = true;
}

void ObjectTransform::SetPosition(const float3& pos){
    Position = pos;
    has_changed = true;
}



float3 ObjectTransform::ToWorldPoint(float3 p) const {
    return TransformVector(ihat, jhat, khat, p) + Position;
}

float3 ObjectTransform::ToLocalPoint(float3 world_point) const {
    float3 relative = world_point - Position;
    return TransformVector(ihat_inv, jhat_inv, khat_inv, relative);
}

void ObjectTransform::UpdateBasisVectors()
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
    ihat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, ihat_roll);
    jhat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, jhat_roll);
    khat = TransformVector(ihat_pitchYaw, jhat_pitchYaw, khat_pitchYaw, khat_roll);

    // Precompute inverse basis
    ihat_inv = float3(ihat.x, jhat.x, khat.x);
    jhat_inv = float3(ihat.y, jhat.y, khat.y);
    khat_inv = float3(ihat.z, jhat.z, khat.z);
}

float3 ObjectTransform::TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v)
{
    return float3(
        v.x * ihat.x + v.y * jhat.x + v.z * khat.x,
        v.x * ihat.y + v.y * jhat.y + v.z * khat.y,
        v.x * ihat.z + v.y * jhat.z + v.z * khat.z
    );
}
