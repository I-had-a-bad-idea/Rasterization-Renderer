#include "Maths.h"
#include <string>
#include <vector>
#include <sstream>
#include "StringHelper.h"
#include "Object.h"



// not so efficient and incomplete obj parser


Object::Object(std::vector<float3> points, std::vector<float3> colors, float3 position, float3 rotation){
    Points = points;
    Triangle_colors = colors;
    Obj_Transform.Position = position;
    Obj_Transform.Position.y *= 1;
    Obj_Transform.Pitch = rotation.x;
    Obj_Transform.Yaw = rotation.y;
}