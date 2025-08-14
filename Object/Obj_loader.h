#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "Object.h"
#include "StringHelper.h"
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

class ObjLoader {
public:
    static std::vector<float3> load_obj_file(std::string obj_string);
    static Object load_object(std::string path, float3 position, float3 rotatation);
};

#endif

