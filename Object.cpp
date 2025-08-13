#include "Maths.h"
#include <string>
#include <vector>
#include <sstream>
#include "StringHelper.h"
#include "Object.h"



// not so efficient and incomplete obj parser
std::vector<float3> ObjLoader::load_obj_file(std::string objString) {
    std::vector<float3> allPoints;
    std::vector<float3> trianglePoints; // each set of 3 points is a triangle

    std::istringstream stream(objString);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.rfind("v ", 0) == 0) { // vertex position
            auto parts = StringHelper::split(line.substr(2), ' ');
            if (parts.size() < 3) continue;

            float x = std::stof(parts[0]);
            float y = std::stof(parts[1]);
            float z = std::stof(parts[2]);
            allPoints.emplace_back(x, y, z);
        }
        else if (line.rfind("f ", 0) == 0) { // face indices
            auto faceIndexGroups = StringHelper::split(line.substr(2), ' ');

            for (int i = 0; i < static_cast<int>(faceIndexGroups.size()); i++) {
                auto indexGroupStr = StringHelper::split(faceIndexGroups[i], '/');
                int pointIndex = std::stoi(indexGroupStr[0]) - 1;

                if (i >= 3) // n-gon triangle fan handling
                    trianglePoints.push_back(trianglePoints[trianglePoints.size() - (3 * i - 6)]);
                if (i >= 3)
                    trianglePoints.push_back(trianglePoints[trianglePoints.size() - 2]);

                trianglePoints.push_back(allPoints[pointIndex]);
            }
        }
    }
    return trianglePoints;
}

Object::Object(std::vector<float3> points, std::vector<float3> colors, float3 position, float3 rotation){
    Points = points;
    Triangle_colors = colors;
    Obj_Transform.Position = position;
    Obj_Transform.Position.y *= 1;
    Obj_Transform.Pitch = rotation.x;
    Obj_Transform.Yaw = rotation.y;
}