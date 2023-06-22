#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Graphics/RenderTypes.hpp"

namespace Wigner {
    class OrthographicCamera {
        public:
            OrthographicCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, f32 width, f32 height);
            OrthographicCamera(glm::vec3 position, glm::vec3 target, f32 width, f32 height);
            ~OrthographicCamera();
            inline glm::mat4 ViewProjectionMatrix() { return m_Projection * m_View; }
        private:
            glm::mat4 m_Transform;
            glm::mat4 m_View;
            glm::mat4 m_Projection;
    };
}