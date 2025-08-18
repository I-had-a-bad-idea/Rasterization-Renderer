#include "Object.h"

Object::Object(ObjectMesh mesh, std::shared_ptr<ObjectShader> shader, std::string name, std::vector<float3> triangle_colors, float3 position, float3 rotation){
    Mesh = mesh;
    Object_Shader = std::move(shader);
    Triangle_colors = triangle_colors;
    Name = name;
    Obj_Transform.Position = position;
    Obj_Transform.Rotation = rotation;
}

void Object::process_object(float2 target_size, Cam camera){
    Mesh.process_model(Obj_Transform, target_size, camera);
}