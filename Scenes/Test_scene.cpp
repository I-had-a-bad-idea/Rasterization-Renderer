#include "Test_scene.h"
#include <SDL2/SDL.h>

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

    // Mouse handling
    int mouseX, mouseY;
    const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    static int lastMouseX = mouseX;
    static int lastMouseY = mouseY;

    if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        SDL_ShowCursor(SDL_DISABLE);
        float2 mouse_delta(
            static_cast<float>(mouseX - lastMouseX) / target.Width * mouse_sensitivity,
            static_cast<float>(mouseY - lastMouseY) / target.Width * mouse_sensitivity
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
    } else {
        SDL_ShowCursor(SDL_ENABLE);
    }

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Keyboard handling
    const float camera_speed = 5;
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    float3 move_delta(0, 0, 0);
    auto [cam_right, cam_up, cam_forward] = camera_transform.GetBasisVectors();
    
    if (keyState[SDL_SCANCODE_W]) move_delta += cam_forward;
    if (keyState[SDL_SCANCODE_S]) move_delta -= cam_forward;
    if (keyState[SDL_SCANCODE_A]) move_delta -= cam_right;
    if (keyState[SDL_SCANCODE_D]) move_delta += cam_right;

    camera_transform.SetPosition(camera_transform.GetPosition() + move_delta * camera_speed * delta_time);
    // camera_transform.SetPosition(float3(camera_transform.GetPosition().x, 1, camera_transform.GetPosition().z)); // optional fixed Y
}
