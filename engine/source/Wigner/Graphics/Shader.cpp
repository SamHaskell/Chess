#include "Wigner/Graphics/Shader.hpp"

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

        int  success;
        char infoLog[512];
        glGetShaderiv(v, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(v, 512, NULL, infoLog);
            LOG_ERROR("Shader Compilation Error: %s", infoLog);
        }

        f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &frag_shader_code, NULL);
        glCompileShader(f);

        glGetShaderiv(f, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(f, 512, NULL, infoLog);
            LOG_ERROR("Shader Compilation Error: %s", infoLog);
        }

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

    void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
        i32 loc = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetUniformInt(const std::string& name, i32 value) {
        i32 loc = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1i(loc, value);
    }
}