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

class ObjLoader {
public:
    static std::tuple<std::vector<float3>, std::vector<float3>, std::vector<float2>> load_obj_file(std::string obj_string);
    static Object load_object(std::string path, float3 position, float3 rotatation, std::string name, std::shared_ptr<ObjectShader> shader);
};

#endif

