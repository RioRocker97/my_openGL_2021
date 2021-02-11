#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 baseColor;
layout (location = 2) in vec2 aTexCord;
out vec3 commonColor;
out vec2 TextCord;

void main()
{
    gl_Position = vec4(aPos,1.0);
	commonColor = baseColor;
    TextCord = aTexCord;
}