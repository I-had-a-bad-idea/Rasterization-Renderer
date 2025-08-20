#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "Vector.h"
#include "RenderTarget.h"
#include "Object.h"
#include "Maths.h"
#include "Scene.h"
#include "Threadpool.h"
#include "Object_mesh.h"

class Rasterizer{
    public:
        static void write_image_to_file(std::vector<float3>& image, int width, int height, const std::string& name);
        static void Render(Scene& scene, RenderTarget& target);
        
    
};




#endif