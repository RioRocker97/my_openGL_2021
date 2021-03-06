#version 330 core

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
void main()
{
    vec3 ambientColor = lightColor * 0.1
    FragColor = vec4(lightColor * objColor,1.0);
}