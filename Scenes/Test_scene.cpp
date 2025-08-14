#include "Test_scene.h"

void TestScene::Setup(){
    Object floor(ObjLoader::load_object("/Objects/Plane.obj", float3(0, -2, 1), float3(0, 0, 0)));
    Object monkey(ObjLoader::load_object("/Objects/Monkey.obj", float3(0, 0, 3), float3(0, 3.141592, 0)));
    Object cube0(ObjLoader::load_object("/Objects/Cube.obj", float3(3, 2, 5), float3(0, 0, 0)));
    Object cube1(ObjLoader::load_object("/Objects/Cube.obj", float3(-3, 2, -5), float3(0, 0, 0)));
    camera.Fov = 60;
    objects = {floor, monkey, cube0, cube1};
}


void TestScene::Update(RenderTarget& target, float delta_time) {
    for (auto& obj : objects) {
        obj.Obj_Transform.Pitch += 3 * delta_time; 
    }
    
    float camera_speed = 5;
    ObjectTransform& camera_transform(camera.CamTransform);
    if(IsKeyDown(KEY_W)) camera_transform.Position.z += camera_speed * delta_time;
    if(IsKeyDown(KEY_S)) camera_transform.Position.z -= camera_speed * delta_time;
    if(IsKeyDown(KEY_A)) camera_transform.Position.x -= camera_speed * delta_time;
    if(IsKeyDown(KEY_D)) camera_transform.Position.x += camera_speed * delta_time;
}
