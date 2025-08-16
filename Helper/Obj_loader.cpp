
#include "Obj_loader.h"



Object ObjLoader::load_object(std::string path, float3 position, float3 rotation, std::string name){
    std::string obj_path = std::filesystem::current_path().string() + path;
    std::string obj_string = StringHelper::readFileToString(obj_path);
    std::tuple<std::vector<float3>, std::vector<float3>, std::vector<float2>> mesh_data = load_obj_file(obj_string);
    std::vector<float3> object_points = std::get<0>(mesh_data);
    std::vector<float3> object_normals = std::get<1>(mesh_data);
    std::vector<float2> texture_cords = std::get<2>(mesh_data);

    if (object_points.empty()) {
       std::cerr << "Failed to load model or model is empty!" << std::endl;
       throw std::runtime_error("Failed to load model: " + path);
    }

    // Create random colors
    std::vector<float3> triangle_colors(object_points.size() / 3);
    for(int i = 0; i < object_points.size() / 3; i++){
        triangle_colors[i] = Math::random_color();
    }
    ObjectMesh mesh(object_points, object_normals, texture_cords);
    return Object(mesh, name, triangle_colors, position, rotation);
}


// not so efficient obj parser

std::tuple<std::vector<float3>, std::vector<float3>, std::vector<float2>> ObjLoader::load_obj_file(std::string objString) {
    std::vector<float3> allPoints;
    std::vector<float3> trianglePoints; // each set of 3 points is a triangle
    std::vector<float3> normals;
    std::vector<float2> texture_cords;

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
        else if(line.rfind("vt", 0) == 0){
            auto parts = StringHelper::split(line.substr(3), ' ');
            float x = std::stof(parts[0]);
            float y = std::stof(parts[1]);
            texture_cords.emplace_back(x, y);
        }
        else if(line.rfind("vn", 0) == 0){
            auto parts = StringHelper::split(line.substr(3), ' ');
            float x = std::stof(parts[0]);
            float y = std::stof(parts[1]);
            float z = std::stof(parts[2]);
            normals.emplace_back(x, y, z);
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
    return std::make_tuple(trianglePoints, normals, texture_cords);
}