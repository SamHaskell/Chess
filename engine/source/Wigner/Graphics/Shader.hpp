#pragma once

#include "Wigner/Base/Base.hpp"

#include "glad/glad.h"

namespace Wigner {
    class Shader {
        public:
            static std::shared_ptr<Shader> Create(std::string vert, std::string frag);

            Shader(std::string vert, std::string frag);
            ~Shader();

            inline void Bind() { glUseProgram(m_ShaderID); }
        private:
            u32 m_ShaderID;
    };
}