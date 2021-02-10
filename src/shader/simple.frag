#version 330 core

in vec3 commonColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(commonColor,1.0);
}