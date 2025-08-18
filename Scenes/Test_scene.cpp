#include "Test_scene.h"

void TestScene::Setup(){
    Object floor(ObjLoader::load_object("/Objects/Plane.obj", float3(0, -2, 1), float3(0, 0, 0), "floor"));
    Object monkey(ObjLoader::load_object("/Objects/Monkey.obj", float3(0, 0, 3), float3(0, 3.141592, 0), "monkey"));
    Object cube0(ObjLoader::load_object("/Objects/Cube.obj", float3(3, 2, 5), float3(0, 0, 0), "cube0"));
    Object cube1(ObjLoader::load_object("/Objects/Cube.obj", float3(-3, 2, -5), float3(0, 0, 0), "cube1"));
    camera.Fov = 60;
    objects = {floor, monkey, cube0, cube1};
}



void TestScene::Update(RenderTarget& target, float delta_time) {
    const float mouse_sensitivity = 2;

    ObjectTransform& camera_transform(camera.CamTransform);

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        HideCursor();
        Vector2 raylib_mouse_delta = GetMouseDelta();
        float2 mouse_delta(
            raylib_mouse_delta.x / target.Width * mouse_sensitivity,
            raylib_mouse_delta.y / target.Width * mouse_sensitivity
        );

        // Get current rotation
        float3 rot = camera_transform.GetRotation();

        // Apply mouse movement
        rot.x = std::clamp(rot.x - mouse_delta.y,
                           Math::degrees_to_radians(-85),
                           Math::degrees_to_radians(85));
        rot.y -= mouse_delta.x;

        // Update (this recalculates basis vectors)
        camera_transform.SetRotation(rot);
    }

    const float camera_speed = 5;

    float3 move_delta(0, 0, 0);
    auto [cam_right, cam_up, cam_forward] = camera_transform.GetBasisVectors();
    if (IsKeyDown(KEY_W)) move_delta += cam_forward;
    if (IsKeyDown(KEY_S)) move_delta -= cam_forward;
    if (IsKeyDown(KEY_A)) move_delta -= cam_right;
    if (IsKeyDown(KEY_D)) move_delta += cam_right;

    camera_transform.SetPosition(camera_transform.GetPosition() + move_delta * camera_speed * delta_time);
    // camera_transform.SetPosition(float3(camera_transform.GetPosition().x, 1, camera_transform.GetPosition().z)); // optional fixed Y
}
