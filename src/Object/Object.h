#ifndef Object_H
#define Object_H

#include "Vector.h"
#include <vector>
#include <string>
#include "ObjectTransform.h"
#include "Object_mesh.h"
#include <memory>
#include "Shaders.h"

class Object {
public:
    ObjectMesh Mesh;
    std::shared_ptr<ObjectShader> Object_Shader;
    std::string Name;
    std::vector<float3> Triangle_colors;
    ObjectTransform Obj_Transform;

    void process_object(float2 target_size, Cam camera);
    Object(ObjectMesh mesh, std::shared_ptr<ObjectShader> shader, std::string name, std::vector<float3> triangle_colors, float3 position, float3 rotation);
};

#endif