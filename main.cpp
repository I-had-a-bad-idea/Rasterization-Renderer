#include <iostream>
#include <string>
#include <filesystem>
#include "Object.h"
#include "Vector.h"
#include "StringHelper.h"
#include "Maths.h"
#include "RenderTarget.h"
#include "Rasterizer.h"
#include "raylib.h"

Object load_object(std::string path, float3 position, float3 rotatation){
    // Load model data
    std::string obj_path = std::filesystem::current_path().string() + path;
    std::string obj_string = StringHelper::readFileToString(obj_path);
    std::vector<float3> object_points = ObjLoader::load_obj_file(obj_string);

   if (object_points.empty()) {
       std::cerr << "Failed to load model or model is empty!" << std::endl;
       throw std::runtime_error("Failed to load model: " + path);
   }

    // Create random colors
    std::vector<float3> triangle_colors(object_points.size() / 3);
    for(int i = 0; i < object_points.size() / 3; i++){
        triangle_colors[i] = Math::random_color();
    }
    return Object(object_points, triangle_colors, position, rotatation);
}

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
    int width = 256 * 4;  // Increased resolution for better visibility
    int height = 256 * 4;
    float fov = 90; // FOV in degrees

    Object monkey(load_object("/Objects/Monkey.obj", float3(0, 0, 3), float3(0, 3.141592, 0)));
    Object cube0(load_object("/Objects/Cube.obj", float3(3, 2, 5), float3(0, 0, 0)));
    Object cube1(load_object("/Objects/Cube.obj", float3(-3, 2, 5), float3(0, 0, 0)));


    RenderTarget render_target(width, height);

    std::vector<Object> objects = {monkey, cube0, cube1};

    Run(render_target, objects, fov);  // Pass the correct FOV value
//  Rasterizer::Render(objects, render_target, fov);
//  Rasterizer::write_image_to_file(render_target.color_buffer, render_target.Width, render_target.Height, "Color");
    
    std::cout << "Rendering complete! Check Test.bmp" << std::endl;
    return 0;
}

//hecx9hs46g@xkxkud.com