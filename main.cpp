#include <iostream>
#include <string>
#include <filesystem>
#include "Object/Object.h"
#include "Math/Vector.h"
#include "Helper/StringHelper.h"
#include "Math/Maths.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Rasterizer.h"
#include "Obj_loader.h"
#include "Test_scene.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


// Convert RenderTarget to SDL texture pixels (RGBA8888)
void ToFlatByteArray(RenderTarget &renderTarget, std::vector<Uint32> &data) {
    data.resize(renderTarget.color_buffer.size());

    for (size_t i = 0; i < renderTarget.color_buffer.size(); i++) {
        float3 &col = renderTarget.color_buffer[i];
        Uint8 r = static_cast<Uint8>(std::clamp(col.r() * 255.0f, 0.0f, 255.0f));
        Uint8 g = static_cast<Uint8>(std::clamp(col.g(), 0.0f, 1.0f) * 255.0f);
        Uint8 b = static_cast<Uint8>(std::clamp(col.b(), 0.0f, 1.0f) * 255.0f);
        Uint8 a = 255;
        data[i] = (a << 24) | (b << 16) | (g << 8) | r; // SDL_PIXELFORMAT_RGBA8888
    }
}

void Run(RenderTarget &target, Scene &scene, float fov) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Rasterization-Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        target.Width,
        target.Height,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        target.Width,
        target.Height
    );

    std::vector<Uint32> pixels(target.Width * target.Height);
    bool running = true;
    Uint32 lastTime = SDL_GetTicks();
    int frameCount = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        // Update world
        Uint32 now = SDL_GetTicks();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;

        scene.Update(target, deltaTime);
        Rasterizer::Render(scene, target);

        // FPS counter
        frameCount++;
        static Uint32 fpsTimer = SDL_GetTicks();
        if (now - fpsTimer >= 1000) {
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
            fpsTimer = now;
        }

        // Copy rasterizer output to SDL texture
        ToFlatByteArray(target, pixels);
        SDL_UpdateTexture(texture, nullptr, pixels.data(), target.Width * sizeof(Uint32));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main(int argc, char* argv[]) {
    int width = 960;
    int height = 540;
    float fov = 90; // FOV in degrees

    TestScene scene;
    scene.Setup(); 

    RenderTarget render_target(width, height);
    Run(render_target, scene, fov);
    
    return 0;
}
