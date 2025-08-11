#include "Maths.h"
#include <string>
#include <vector>
#include <sstream>
#include "StringHelper.cpp"


class ObjLoader{
    public:

        // not so efficient and incomplete obj parser
        static std::vector<float3> load_obj_file(std::string obj_string){
            std::vector<float3> all_points;
            std::vector<float3> triangle_points;  // each set of 3 points is a triangle

            std::istringstream iss(obj_string);
            // for every line in the string
            for (std::string line; std::getline(iss, line); ){
                if(line._Starts_with("v ")){ // vertex position
                    auto parts = StringHelper::split(line.substr(2), ' ');
                    if(parts.size() < 3) continue;

                    float x = std::stof(parts[0]);
                    float y = std::stof(parts[1]);
                    float z = std::stof(parts[2]);
                    all_points.emplace_back(float3(x, y, z));
                }
                else if(line._Starts_with("f ")){ //face indices
                    auto face_index_groups = StringHelper::split(line.substr(2), ' ');
                    
                    for(int i = 0; i < face_index_groups.size(); i++){
                        auto index_group_str = StringHelper::split(face_index_groups[i], '/');
                        int point_index = std::stoi(index_group_str[0]) -1;

                        //n-gon triangle fan handling
                        if(i >= 3){
                            triangle_points.emplace_back(triangle_points[triangle_points.size() - (3 * i - 6)]);
                        }
                        if(i >= 3){
                            triangle_points.emplace_back(triangle_points[triangle_points.size() - 2]);
                        }
                        triangle_points.emplace_back(all_points[point_index]);


                    }
                }
            }

        return triangle_points;
        }

};


class Model{
    public:
        std::vector<float3> Points;
        std::vector<float3> Triangle_colors;

        Model(std::vector<float3> points, std::vector<float3> colors){
            Points = points;
            Triangle_colors = colors;

        }
};