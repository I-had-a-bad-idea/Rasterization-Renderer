#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "Object.h"
#include "StringHelper.h"
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "Object_mesh.h"
#include "Maths.h"
#include "Shaders.h"
#include "Image_loader.h"

class ObjLoader {
public:
    static std::tuple<std::vector<float3>, std::vector<float3>, std::vector<float2>> load_obj_file(std::string obj_string);
    static Object load_object(std::string obj_path, std::string texture_path, float3 position, float3 rotatation, std::string name);
};

#endif

