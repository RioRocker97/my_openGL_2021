#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <vector>

using namespace glm;
using namespace std;

struct Vertex{
    vec3 position;
    vec3 normal;
    vec2 textCord;
    vec3 tangent;
    vec3 bitangent;
};
//ok clearly we need to build from small to big
//each file contain a scene
//a scene contain one or more 3d model 
// each model contain one or more mesh
//each mesh contain many indices
//indices is another way to process vertice without using same value . much faster and more efficient
//i already build mesh.h without texture include . just want to import basic 3d geometry first  
//will do later 
class Model{
    public:
        Model(const char *path){
        }
        static void setDIR(const char *path){
            MAIN_DIRECTORY = path;
        }
    private:
        static const char * MAIN_DIRECTORY;
};
#endif