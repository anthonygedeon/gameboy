#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_0;

void main()
{
    vec4 color = texture(texture_0, TexCoord);

    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    vec3 green = vec3(0.0, 1.0, 0.0);

    FragColor = vec4(mix(color.rgb, green * gray, 0.6), color.a);
}