#version 330 core

in vec3 commonColor;
in vec2 TextCord;

out vec4 FragColor;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float blending_rate;
void main()
{
    FragColor = mix(texture(myTexture1,TextCord),texture(myTexture2,TextCord),blending_rate);
}