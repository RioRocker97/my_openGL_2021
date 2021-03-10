#version 330 core
out vec4 FragColor;
uniform vec3 lightSource;
void main()
{
    FragColor = vec4(lightSource,1.0);
}