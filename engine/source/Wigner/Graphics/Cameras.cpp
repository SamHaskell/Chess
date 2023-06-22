#include "Wigner/Graphics/Cameras.hpp"

namespace Wigner
{
    OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up, f32 width, f32 height)
    {
        m_Transform = glm::mat4(1.0f);
        m_View = glm::lookAt(position, target, up);
        m_Projection = glm::ortho(-width/2.0, width/2.0, -height/2.0, height/2.0);
    }

    OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 target, f32 width, f32 height)
    {
        OrthographicCamera(position, target, {0.0f, 1.0f, 0.0f}, width, height);
    }

    OrthographicCamera::~OrthographicCamera()
    {
    }
}