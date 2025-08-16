#include "Object.h"

Object::Object(ObjectMesh mesh, std::string name, std::vector<float3> triangle_colors, float3 position, float3 rotation){
    Mesh = mesh;
    Triangle_colors = triangle_colors;
    Name = name;
    Obj_Transform.Position = position;
    Obj_Transform.Pitch = rotation.x;
    Obj_Transform.Yaw = rotation.y;
}