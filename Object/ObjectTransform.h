#ifndef OBJECT_TRANSFORM_H
#define OBJECT_TRANSFORM_H

#include "Vector.h"
#include <tuple>

class ObjectTransform {
public:
    float3 Rotation;
    float3 Position;

    ObjectTransform();                 
    ObjectTransform(float3 position, float3 rotation);         

    // Accessors for position
    void SetPosition(const float3& pos) { Position = pos; }
    float3 GetPosition() const { return Position; }

    // Accessors for rotation (auto-updates basis vectors)
    void SetRotation(const float3& rot);
    float3 GetRotation() const { return Rotation; }

    // Transform a point from local space to world space
    float3 ToWorldPoint(float3 p) const;
    float3 ToLocalPoint(float3 world_point) const;

    // Get cached basis vectors
    std::tuple<float3, float3, float3> GetBasisVectors() const { return {ihat, jhat, khat}; }
    std::tuple<float3, float3, float3> GetInverseBasisVectors() const { return {ihat_inv, jhat_inv, khat_inv}; }

private:

    // Cached basis vectors
    float3 ihat, jhat, khat;           
    float3 ihat_inv, jhat_inv, khat_inv; 

    void UpdateBasisVectors();

    // Move each coordinate of given vector along the corresponding basis vector
    static float3 TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v);
};

#endif
