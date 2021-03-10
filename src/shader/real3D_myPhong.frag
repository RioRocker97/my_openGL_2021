#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct SimpleLight{
    vec3 lightSource;
    vec3 direction;
};

in vec3 Normal;
in vec3 frag_pos;
in vec2 TextCord;
out vec4 FragColor;

uniform SimpleLight light;
uniform vec3 viewPos;
uniform Material myMat;
void main()
{
    // for ambient
    vec3 ambientColor = light.lightSource * vec3(texture(myMat.diffuse,TextCord));
    //for diffuse
    vec3 new_normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);  
    float diff = max(dot(new_normal, lightDir), 0.0);
    vec3 diffuseColor = diff * light.lightSource * vec3(texture(myMat.diffuse,TextCord));
    //for specular
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, new_normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),myMat.shininess);
    vec3 specularColor = vec3(texture(myMat.specular,TextCord)) * spec * light.lightSource;  

    vec3 result = ambientColor + diffuseColor + specularColor;
    FragColor = vec4(result,1.0);
}