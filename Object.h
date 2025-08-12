#ifndef Object_H
#define Object_H

#include "Vector.h"
#include <vector>
#include <string>
#include "ObjectTransform.h"

class ObjLoader {
public:
    static std::vector<float3> load_obj_file(std::string obj_string);
};

class Object {
public:
    std::vector<float3> Points;
    std::vector<float3> Triangle_colors;
    ObjectTransform Object_transform;

    Object(std::vector<float3> points, std::vector<float3> colors);
};

#endif