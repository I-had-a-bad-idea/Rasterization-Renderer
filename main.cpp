#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
#include <SDL2/SDL_opengl.h>

// Convert RenderTarget to byte array (RGBA8888)
void ToFlatByteArray(RenderTarget &renderTarget, std::vector<Uint8> &data) {
    data.resize(renderTarget.color_buffer.size() * 4);

    for (size_t i = 0; i < renderTarget.color_buffer.size(); i++) {
        float3 &col = renderTarget.color_buffer[i];
        Uint8 r = static_cast<Uint8>(std::clamp(col.r(), 0.0f, 1.0f) * 255.0f);
        Uint8 g = static_cast<Uint8>(std::clamp(col.g(), 0.0f, 1.0f) * 255.0f);
        Uint8 b = static_cast<Uint8>(std::clamp(col.b(), 0.0f, 1.0f) * 255.0f);
        Uint8 a = 255;

        data[i * 4 + 0] = r;
        data[i * 4 + 1] = g;
        data[i * 4 + 2] = b;
        data[i * 4 + 3] = a;
    }
}

void Run(RenderTarget &target, Scene &scene) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return;
    }

    // Request an OpenGL 2.1 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Rasterization-Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        target.Width,
        target.Height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Enable relative mouse mode (always captured)
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);

    // Create OpenGL texture
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, target.Width, target.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    std::vector<Uint8> pixels(target.Width * target.Height * 4);

    bool running = true;
    Uint32 lastTime = SDL_GetTicks();
    int frameCount = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // Update scene
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

        // Copy rasterizer output into byte array
        ToFlatByteArray(target, pixels);

        // Upload to OpenGL texture
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, target.Width, target.Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        // Draw fullscreen quad
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        SDL_GL_SwapWindow(window);
    }

    glDeleteTextures(1, &texID);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    int width = 960;
    int height = 540;

    TestScene scene;
    scene.Setup(); 

    RenderTarget render_target(width, height);
    Run(render_target, scene);
    
    return 0;
}
