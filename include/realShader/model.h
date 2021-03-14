#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <realShader/mesh.h>

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <vector>

using namespace glm;
using namespace std;

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
        //static const char * MAIN_DIRECTORY;
        vector<Mesh> all_mesh;
        string filename;


        Model(const char *main_path,const char *path){
            Assimp::Importer importer;
            filename = main_path;
            filename.append(path);
            const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | 
                                    aiProcess_GenSmoothNormals | aiProcess_FlipUVs | 
                                    aiProcess_CalcTangentSpace);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
                printf("ERROR::ASSIMP:: %s",importer.GetErrorString());
                return;
            }

            //load node from scene
            processNode(scene->mRootNode,scene);
        }
        void render(Shader shader,vec3 xyz,bool isFlip){
            for(unsigned int i = 0; i < all_mesh.size(); i++)
                all_mesh[i].render(shader,xyz,isFlip);
        }
    private:
        void processNode(aiNode* node,const aiScene* scene){
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                //load each mesh in each node
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                all_mesh.push_back(processMesh(mesh, scene));
            }
            // if children have nodes . it recursive this function to load those children meshes again
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }
        Mesh processMesh(aiMesh* mesh,const aiScene* scene){
            vector<Vertex> verticles;
            vector<unsigned int> indices;
            
            for(unsigned int i = 0; i < mesh->mNumVertices; i++){
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    // is this mean for every model i loaded can have 8 different uv map ?
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.textCord = vec;
                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.bitangent = vector;
                }
                else
                    vertex.textCord = glm::vec2(0.0f, 0.0f);

                verticles.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++){
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);        
            }
            // will do material loading from file later 
            //from now i just want it to load basic model
            return Mesh(verticles,indices);
        }
};
#endif