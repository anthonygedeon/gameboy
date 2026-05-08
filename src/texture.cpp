#include "../include/texture.hpp"
#include <glad/gl.h>
#include <iostream>
#include "../include/stb_image.hpp"


Texture::Texture(std::string_view path)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.data(), &m_width, &m_height, &m_channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::FAILED_LOADING_TEXTURE" << path << std::endl;
        return;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}


void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}


void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
