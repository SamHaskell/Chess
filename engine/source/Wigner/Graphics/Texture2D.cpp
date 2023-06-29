#include "Wigner/Graphics/Texture2D.hpp"

#include "stb_image.h"

namespace Wigner
{
    Texture2D::Texture2D(const std::string &path)
    {
		stbi_set_flip_vertically_on_load(1);
        u8* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NumChannels, 0);
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            LOG_INFO("Image loaded with dimensions: %i, %i", m_Width, m_Height);
        }
        else
        {
            LOG_ERROR("Failed to load texture: %s", path.c_str());
            // TODO: Default to some debug texture???
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
    {
        return std::make_shared<Texture2D>(path);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }
}