#pragma once
#include "Vector.h"
#include <tuple>

class Transform
{
public:
    float Yaw; // Rotation around y axis
    float Pitch;

    Transform();                 
    Transform(float yaw, float pitch);         
    
    // Transform a point from local space to world space
    float3 ToWorldPoint(float3 p);
    
private:
    // Calculate right/up/forward vectors (i, j, k)
    std::tuple<float3, float3, float3> GetBasisVectors();
    
    // Move each coordinate of given vector along the corresponding basis vector
    static float3 TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v);
};