#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <vector>
#include <fstream>
#include <SDL2/SDL_image.h>

class ImageLoader{
    public:
        static std::vector<unsigned char> png_file_to_bytes(const std::string& filePath);
};


#endif