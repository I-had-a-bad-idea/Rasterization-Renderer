#ifndef OBJECT_TRANSFORM_H
#define OBJECT_TRANSFORM_H

#include "Vector.h"
#include <tuple>

class ObjectTransform{
    public:
        float Yaw; // Rotation around y axis
        float Pitch;
        float3 Position;

        ObjectTransform();                 
        ObjectTransform(float3 position, float yaw, float pitch);         

        // Transform a point from local space to world space
        float3 ToWorldPoint(float3 p);
        float3 ToLocalPoint(float3 world_point);

        // Calculate right/up/forward vectors (i, j, k)
        std::tuple<float3, float3, float3> GetBasisVectors();
        std::tuple<float3, float3, float3> GetInverseBasisVectors();

    private:
        

        // Move each coordinate of given vector along the corresponding basis vector
        static float3 TransformVector(float3 ihat, float3 jhat, float3 khat, float3 v);
};

#endif