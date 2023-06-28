#pragma once

#include "Wigner/Base/Base.hpp"
#include <string>

#include "glad/glad.h"

namespace Wigner
{
    class Texture2D
    {
    public:
        Texture2D(const std::string& path);
        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        ~Texture2D();

        inline void Bind() const { glBindTexture(GL_TEXTURE_2D, m_TextureID); }

    private:
        u32 m_TextureID;
        i32 m_Width;
        i32 m_Height;
        i32 m_NumChannels;
    };
}