#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "StringHelper.h"
// Read entire file into a string
std::string StringHelper::readFileToString(const std::string& path){
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
// Split string by delimiter
std::vector<std::string> StringHelper::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
    }

