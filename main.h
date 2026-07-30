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
void ToFlatByteArray(RenderTarget &renderTarget, std::vector<Uint8> &data);

// Main renderer loop
void Run(RenderTarget &target, Scene &scene);