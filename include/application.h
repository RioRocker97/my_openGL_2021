#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <math.h>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <realShader/camera.h>
#include <realShader/texture.h>
#include <realShader/shader.h>

SingleCamera cam1 = SingleCamera(5.0f, 0.0f,-10.0f);
int SingleCamera::id = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void walkAround(GLFWwindow *window){
    vec3 cameraPos = cam1.getPOS();
	vec3 cameraFront = cam1.getFRONT();
	vec3 cameraUp = cam1.getUP();
    float cameraSpeed = 0.001f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	cam1.setPOS(cameraPos);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
	cam1.move(xpos,ypos,0.6f);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)

{
	cam1.setFOV(cam1.getFOV()-yoffset);
}

/* assimp function for loading model*/
void processNode(aiNode *node, const aiScene *scene);
void processMesh(aiMesh *mesh, const aiScene *scene);
/*----------------------------*/

int myApp(GLFWwindow* WIN_APP,int WIN_WIDTH,int WIN_HEIGHT,const char* WIN_PATH){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	if(WIN_APP == NULL){
		printf("Can't create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(WIN_APP);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("Can't create GLAD");
		return -1;
	}

	glfwSetFramebufferSizeCallback(WIN_APP, framebuffer_size_callback);
	glfwSetCursorPosCallback(WIN_APP, mouse_callback);
    glfwSetScrollCallback(WIN_APP, scroll_callback);
	glfwSetInputMode(WIN_APP, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    printf("%s\n",WIN_PATH);
    Shader basic(WIN_PATH,"/resource/GLSL/basic3.vert","/resource/GLSL/basic3.frag");
    Shader lightShader(WIN_PATH,"/resource/GLSL/simple.vert","/resource/GLSL/simple.frag");

    /* try to understand Assimp 
    Assimp::Importer importer;
    string p = WIN_PATH;
    p.append("/resource/objects/ball.obj");
    const aiScene* scene = importer.ReadFile(p.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        printf("ERROR::ASSIMP:: %s",importer.GetErrorString());
        return -1;
    }
    --------------- */
    printf("---------------------------------------------------------\n");
    printf("Hello My world !!!\n");
    printf("---------------------------------------------------------\n");
    basic.use();
    lightShader.use();
    while(!glfwWindowShouldClose(WIN_APP)){
        processInput(WIN_APP);

        glClearColor(0.6f,0.6f,0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glfwSwapBuffers(WIN_APP);
    	glfwPollEvents();   
    }
    basic.destroy();
	lightShader.destroy();

	glfwTerminate();
}

#endif