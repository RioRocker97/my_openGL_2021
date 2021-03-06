#version 330 core
in vec3 Normal;
in vec3 frag_pos;
out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
    // for ambient
    vec3 ambientColor = lightColor * 0.3;
    vec3 new_normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - frag_pos);  
    //for diffuse
    float diff = max(dot(new_normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //for specular
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, new_normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;  

    vec3 result = (ambientColor + diffuse + specular) * objColor;
    FragColor = vec4(result,1.0);
}