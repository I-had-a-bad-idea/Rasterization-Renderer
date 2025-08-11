#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class StringHelper{
    public:
        static std::string readFileToString(const std::string path) {
            std::ifstream file(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        static std::vector<std::string> split(const std::string& s, char delimiter) {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter)) {
                if (!token.empty())
                    tokens.push_back(token);
            }
            return tokens;
            }
};
