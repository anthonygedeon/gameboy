#ifndef SHADER_H
#define SHADER_H

#include <string>


class Shader {
public:
    Shader(std::string_view vertexPath, std::string_view fragmentPath);
    ~Shader();

    unsigned int getId() const { return m_id; }
    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    unsigned int m_id;
};

#endif
