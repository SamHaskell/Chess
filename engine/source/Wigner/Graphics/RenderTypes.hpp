#pragma once

#include "glm/glm.hpp"

namespace Wigner {
    using Color = glm::vec4;

    struct Vertex2D {
        glm::vec2 Position;
        glm::vec2 TexCoord;
    };
}