#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <realShader/shader.h>
#include <realShader/texture.h>

#include <string>
#include <vector>
#include <stdio.h>

using namespace std; 
using namespace glm;

struct Vertex{
    vec3 position;
    vec3 normal;
    vec2 textCord;
    vec3 tangent;
    vec3 bitangent;
};

class Mesh{
    public:
        vector<Vertex> verticles;
        vector<unsigned int> indices;
        unsigned int VAO,VBO,EBO;

        Mesh(vector<Vertex> verticle,vector<unsigned int> indice){
            verticles = verticle;
            indices = indice;

            BuildMesh();
        }
        void render(Shader program,vec3 xyz,bool isFilp){
            mat4 model = mat4(1.0f);
            model = translate(model,xyz);
            if(isFilp)model = rotate(model,radians(180.0f),vec3(0.0f,0.0f,1.0f));
            program.setTransform("model",value_ptr(model));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    private:
        void BuildMesh(){
            glGenVertexArrays(1,&VAO);
            glGenBuffers(1,&VBO);
            glGenBuffers(1,&EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);

            glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(Vertex), &verticles[0], GL_DYNAMIC_DRAW);  

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);	
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);	
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            // vertex texture coords
            glEnableVertexAttribArray(2);	
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCord));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

            glBindVertexArray(0);
        }

};
#endif