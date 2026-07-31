#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "src/Object/Object.h"
#include "src/Math/Vector.h"
#include "src/Helper/StringHelper.h"
#include "src/Math/Maths.h"
#include "src/Rendering/RenderTarget.h"
#include "src/Rendering/Rasterizer.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Convert RenderTarget to byte array (RGBA8888)
void ToFlatByteArray(RenderTarget &renderTarget, std::vector<Uint8> &data);

// Main renderer loop
void Run(RenderTarget &target, Scene &scene);