#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Graphics/RenderTypes.hpp"

namespace Wigner {
    class OrthographicCamera {
        public:
            OrthographicCamera(glm::vec3 position, glm::vec3 target, f32 width, f32 height, glm::vec3 up = {0.0f, 1.0f, 0.0f});
            ~OrthographicCamera();
            void SetFrustumRect(f32 width, f32 height);
            glm::mat4 TransformMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 ProjectionMatrix;
        private:
    };
}