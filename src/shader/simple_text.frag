#version 330 core

in vec3 commonColor;
in vec2 TextCord;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture,TextCord);
}