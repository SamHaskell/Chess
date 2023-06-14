#pragma once

#include "Wigner/Base/Base.hpp"
#include "glm/glm.hpp"

namespace Wigner {
    using Color = glm::vec4;

    struct Vertex2D {
        glm::vec4 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
    };
}