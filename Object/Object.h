#ifndef Object_H
#define Object_H


#include <vector>
#include <string>
#include "Maths.h"
#include "ObjectTransform.h"


class Object {
public:
    std::vector<float3> Points;
    std::vector<float3> Triangle_colors;
    ObjectTransform Obj_Transform;

    Object(std::vector<float3> points, std::vector<float3> colors, float3 position, float3 rotation);
};

#endif