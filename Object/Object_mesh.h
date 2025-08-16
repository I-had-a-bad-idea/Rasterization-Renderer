#ifndef OBJECT_MESH_H
#define OBJECT_MESH_H

#include "Vector.h"
#include <vector>

class ObjectMesh{
    public:
        std::vector<float3> Vertices;
        std::vector<float3> Normals;
        std::vector<float2> Texture_cords;

        ObjectMesh() = default;
        ObjectMesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<float2> texture_cords);
};


#endif