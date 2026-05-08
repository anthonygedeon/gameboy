#ifndef TEXTURE_H
#define TEXTURE_H

#include <string_view>


class Texture {
public:
    Texture(std::string_view path);
    ~Texture();

    unsigned int getId() const { return m_id; }
    void bind(unsigned int slot = 0) const;
    void unbind() const;

private:
    unsigned int m_id;
    int m_width;
    int m_height;
    int m_channels;
};

#endif
