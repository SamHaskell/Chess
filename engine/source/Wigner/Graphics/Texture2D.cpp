#include "Wigner/Graphics/Texture2D.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "stb_image.h"

namespace Wigner {
    Texture2D::Texture2D(const std::string& path) {
        // TODO: TEMP, JUST TO CHECK FILEPATH STUFF
        std::ifstream stream;
        stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            stream.open(path);
            stream.close();
        }
        catch (std::ifstream::failure& e) {
            LOG_ERROR("Texture not read successfully!");
        }

    }

    Texture2D::~Texture2D() {

    }
}