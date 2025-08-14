#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class StringHelper {
public:
    static std::string readFileToString(const std::string& path);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif