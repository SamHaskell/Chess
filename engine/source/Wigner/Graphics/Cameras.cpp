#include "Wigner/Graphics/Cameras.hpp"

namespace Wigner
{
    OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 target, f32 width, f32 height, glm::vec3 up)
    {
        TransformMatrix = glm::mat4(1.0f); // TODO: FIX THIS
        ViewMatrix = glm::lookAt(position, target, up);
        ProjectionMatrix = glm::ortho(0.0f, (f32)width, 0.0f, (f32)height, 0.0f, 1000.0f);
    }

    OrthographicCamera::~OrthographicCamera()
    {
    }
}