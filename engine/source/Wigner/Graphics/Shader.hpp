#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Graphics/RenderTypes.hpp"

#include "glad/glad.h"

namespace Wigner {
    class Shader {
        public:
            static std::shared_ptr<Shader> Create(std::string vert, std::string frag);

            Shader(std::string vert, std::string frag);
            ~Shader();

            inline void Bind() { glUseProgram(m_ShaderID); }
            void SetUniformMat4(const std::string& name, const glm::mat4& value);
        private:
            u32 m_ShaderID;
    };
}