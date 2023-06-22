#include "Wigner/Graphics/Shader.hpp"

#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Wigner
{
    Shader::Shader(std::string vert, std::string frag)
    {
        std::string vert_code, frag_code;
        std::ifstream vert_filestream, frag_filestream;
        vert_filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        frag_filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vert_filestream.open(vert);
            frag_filestream.open(frag);
            std::stringstream vert_stream, frag_stream;
            vert_stream << vert_filestream.rdbuf();
            frag_stream << frag_filestream.rdbuf();
            vert_filestream.close();
            frag_filestream.close();
            vert_code = vert_stream.str();
            frag_code = frag_stream.str();
        }
        catch (std::ifstream::failure& e) {
            LOG_ERROR("Shader file not read successfully!");
        }

        const char* vert_shader_code = vert_code.c_str();
        const char* frag_shader_code = frag_code.c_str();
        u32 v, f;
        
        v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vert_shader_code, NULL);
        glCompileShader(v);

        f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &frag_shader_code, NULL);
        glCompileShader(f);

        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, v);
        glAttachShader(m_ShaderID, f);
        glLinkProgram(m_ShaderID);

        glDeleteShader(v);
        glDeleteShader(f);
    }

    std::shared_ptr<Shader> Shader::Create(std::string vert, std::string frag)
    {
        return std::make_shared<Shader>(vert, frag);
    }

    Shader::~Shader()
    {
    }
}