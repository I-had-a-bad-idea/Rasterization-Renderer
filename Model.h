#ifndef MODEL_H
#define MODEL_H

#include "Vector.h"
#include <vector>
#include <string>

class ObjLoader {
public:
    static std::vector<float3> load_obj_file(const std::string& obj_string);
};

class Model {
public:
    std::vector<float3> Points;
    std::vector<float3> Triangle_colors;

    Model(const std::vector<float3>& points, const std::vector<float3>& colors);
};

#endif