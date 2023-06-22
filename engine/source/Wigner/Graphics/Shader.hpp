#pragma once

#include "Wigner/Base/Base.hpp"

namespace Wigner {
    class Shader {
        public:
            static std::shared_ptr<Shader> Create(std::string vert, std::string frag);

            Shader(std::string vert, std::string frag);
            ~Shader();
        private:
            u32 m_ShaderID;
    };
}