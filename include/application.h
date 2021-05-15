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
#include <realShader/model.h>

SingleCamera cam1 = SingleCamera(0.0f,0.0f,-5.0f);
int SingleCamera::id = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void walkAround(GLFWwindow *window,float compTime){
    vec3 cameraPos = cam1.getPOS();
	vec3 cameraFront = cam1.getFRONT();
	vec3 cameraUp = cam1.getUP();
    float cameraSpeed = 3.0f*compTime; 
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
void setup_world(Shader shader,SingleCamera cam,vec3 single_light){
    shader.use();
    shader.setTransform("projection",value_ptr(cam.getPROJECT()));
    shader.setTransform("view",value_ptr(cam.getVIEW()));
    shader.setInt("myMat.diffuse",0);
    shader.setInt("myMat.specular",1);
    shader.setFloat("myMat.shininess",8.0f);
    shader.setVec3("light.lightSource",1.0f,1.0f,1.0f);
    shader.setVec3("light.lightPosition",single_light.x,single_light.y,single_light.z);
    shader.setFloat("light.constant",  1.0f);
    shader.setFloat("light.linear",    0.07f);
    shader.setFloat("light.quadratic", 0.017f);
    shader.setVec3("viewPos",cam.getPOS().x,cam.getPOS().y,cam.getPOS().z);
}
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
    glfwSetInputMode(WIN_APP, GLFW_STICKY_KEYS, GLFW_TRUE);

    Shader basic(WIN_PATH,"/resource/GLSL/basic3.vert","/resource/GLSL/basic3.frag");
    Shader lightShader(WIN_PATH,"/resource/GLSL/simple.vert","/resource/GLSL/simple.frag");

    Model myBall(WIN_PATH,"/resource/objects/ball.obj");
    Model mySelf(WIN_PATH,"/resource/objects/me.obj");
    Model myFloor(WIN_PATH,"/resource/objects/floor.obj");
    Model myMeat(WIN_PATH,"/resource/objects/cylinder.obj");

    Texture ball_dif(WIN_PATH,"/resource/texture/default.jpg");
    Texture ball_spc(WIN_PATH,"/resource/texture/default_spec.jpg");
    Texture meat_dif(WIN_PATH,"/resource/texture/cylinder_dif.jpg");
    Texture meat_spec(WIN_PATH,"/resource/texture/cylinder_spec.jpg");
    /* try to understand Assimp 
    Assimp::Importer importer;
    string p = WIN_PATH;
    p.append("/resource/objects/ball.obj");
    const aiScene* scene = importer.ReadFile(p.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        printf("ERROR::ASSIMP:: %s",importer.GetErrorString());
        return -1;
    }
    aiNode* node = scene->mRootNode;
    printf("Num of Children : %i\n",node->mNumChildren);
    printf("Num of current Node 's meshs : %i\n",scene->mNumMeshes);
    --------------- */
    printf("---------------------------------------------------------\n");
    printf("Hello My world !!!\n");
    printf("If it successfully loaded, you will see basic shit and floor\n");
    printf("---------------------------------------------------------\n");
    cam1.makeMyProjection((float)WIN_WIDTH,(float)WIN_HEIGHT,55.0f);
    glEnable(GL_DEPTH_TEST);  
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(WIN_APP)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(WIN_APP);
        walkAround(WIN_APP,deltaTime);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        vec3 lightPOS = vec3(0.0f,3.0f,0.0f);
        ball_dif.use(0);
        ball_spc.use(1);
        setup_world(basic,cam1,lightPOS);

        vec3 myBallPOS = vec3(1.0f,0.0f,0.0f);
        myBall.render(basic,myBallPOS);
        mySelf.render(basic,vec3(3.0f,2.0f,0.0f));
        myFloor.render(basic,vec3(0.0f,-1.0f,0.0f));

        meat_dif.use(0);
        meat_spec.use(1);
        myMeat.render(basic,vec3(2.0f,1.0f,2.0f));

        glfwSwapBuffers(WIN_APP);
    	glfwPollEvents();   
    }
    basic.destroy();
	lightShader.destroy();

	glfwTerminate();
}

#endif