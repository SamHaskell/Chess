#pragma once

#include "Wigner/Base/Base.hpp"
#include <string>

#include "glad/glad.h"

namespace Wigner
{
    class Texture2D
    {
    public:
        static std::shared_ptr<Texture2D> Create(const std::string& path);

        Texture2D(const std::string& path);
        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        ~Texture2D();

        inline void Bind() const { glBindTexture(GL_TEXTURE_2D, m_TextureID); }
        inline f32 GetAspectRatio() const { return (f32)m_Width / (f32)m_Height; }

    private:
        u32 m_TextureID;
        i32 m_Width;
        i32 m_Height;
        i32 m_NumChannels;
    };
}