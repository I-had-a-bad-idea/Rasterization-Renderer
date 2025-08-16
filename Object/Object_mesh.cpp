
#include "Object_mesh.h"




ObjectMesh::ObjectMesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<float2> texture_cords){
    Vertices = vertices;
    Normals = normals;
    Texture_cords = texture_cords;
}
