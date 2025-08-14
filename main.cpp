#include <iostream>
#include <string>
#include <filesystem>
#include "Object/Object.h"
#include "Math/Vector.h"
#include "Helper/StringHelper.h"
#include "Math/Maths.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Rasterizer.h"
#include "raylib.h"
#include "Obj_loader.h"




void ToFlatByteArray(RenderTarget &renderTarget, std::vector<Color> &data) {
    data.resize(renderTarget.color_buffer.size());

    for (size_t i = 0; i < renderTarget.color_buffer.size(); i++) {
        float3 &col = renderTarget.color_buffer[i];
        unsigned char r = static_cast<unsigned char>(std::clamp(col.r() * 255.0f, 0.0f, 255.0f));
        unsigned char g = static_cast<unsigned char>(std::clamp(col.g(), 0.0f, 1.0f) * 255.0f);
        unsigned char b = static_cast<unsigned char>(std::clamp(col.b(), 0.0f, 1.0f) * 255.0f);
        data[i] = { r, g, b, 255 }; // Raylib's Color is {unsigned char r,g,b,a;}
    }
}

//TODO make this use a "scene", which also has an update cycle, using this line:   scene.Update(target, GetFrameTime());
void Run(RenderTarget target, std::vector<Object> objects, float fov){
    // Create window
    InitWindow(target.Width, target.Height, "Rasterization-Renderer");
    Texture2D texture(LoadTextureFromImage(GenImageColor(target.Width, target.Height, BLACK)));

    std::vector<Color> textureBytes(target.Width * target.Height * 4); // RGBA

    // Render loop
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        Rasterizer::Render(objects, target, fov);

        // Write rasterizer output to texture and display on window
        ToFlatByteArray(target, textureBytes);
        UpdateTexture(texture, textureBytes.data());

        BeginDrawing();
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();

}


int main() {
    int width = 1920;  // Increased resolution for better visibility
    int height = 1080;
    float fov = 90; // FOV in degrees

    Object flooar(ObjLoader::load_object("/Objects/Plane.obj", float3(0, -2, 1), float3(0, 0, 0)));
    Object monkey(ObjLoader::load_object("/Objects/Monkey.obj", float3(0, 0, 3), float3(0, 3.141592, 0)));
    Object cube0(ObjLoader::load_object("/Objects/Cube.obj", float3(3, 2, 5), float3(0, 0, 0)));
    Object cube1(ObjLoader::load_object("/Objects/Cube.obj", float3(-3, 2, -5), float3(0, 0, 0)));


    RenderTarget render_target(width, height);

    std::vector<Object> objects = {monkey, cube0, cube1};

    Run(render_target, objects, fov);
    
    return 0;
}

//5mp353v1ph@illubd.com