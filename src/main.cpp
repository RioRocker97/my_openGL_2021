#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <myShader/myshader.h>
#include <myShader/mytexture.h>
#include <myShader/mycamera.h>

#define MY_WIDTH 1280
#define MY_HEIGHT 720

using namespace glm;

myCamera cam1 = myCamera(5.0f, 0.0f,-10.0f);
int myCamera::id = 0;

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
	cam1.moveMyCamera(xpos,ypos,0.6f);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam1.setFOV(cam1.getFOV()-yoffset);
}
int main(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	GLFWwindow* mywin = glfwCreateWindow(MY_WIDTH,MY_HEIGHT,"Chang's Sandbox",NULL,NULL); //window area
	if(mywin == NULL){
		printf("Can't create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mywin);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("Can't create GLAD");
		return -1;
	}

	glfwSetFramebufferSizeCallback(mywin, framebuffer_size_callback);
	glfwSetCursorPosCallback(mywin, mouse_callback);
    glfwSetScrollCallback(mywin, scroll_callback);
	glfwSetInputMode(mywin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader realShader("real3D_myPhong.vert","real3D_myPhong.frag");
	Shader mylight("real3D.vert","real3D_light.frag");

	Texture2D myBox("box.jpg",false);
	Texture2D myBox2("box_spec.jpg",false);
	Texture2D myPlane("floor.jpg",false);
	Texture2D myPlane2("floor_spec.jpg",false);
	Texture2D animateMe("cloth.jpg",false);
	Texture2D animateMe2("cloth_spec.jpg",false);

    float mycube_phong[] = {
		//Verticle			  //Normal				//Texture
 		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
	float myplane_phong[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
	};
	vec3 allCube[] = {
		//C
		vec3( 0.0f,  0.0f,  0.0f),
        vec3( 1.1f,  0.0f,  0.0f),
		vec3( 2.2f,  0.0f,  0.0f),
		vec3( 3.3f,  0.0f,  0.0f),
		vec3( 4.4f,  0.0f,  0.0f),
		vec3( 4.4f,  1.1f,  0.0f),
        vec3( 4.4f,  2.2f,  0.0f),
		vec3( 4.4f,  3.3f,  0.0f),
		vec3( 4.4f,  4.4f,  0.0f),
		vec3( 4.4f,  5.5f,  0.0f),
		vec3( 3.3f,  5.5f,  0.0f),
		vec3( 2.2f,  5.5f,  0.0f),
		vec3( 1.1f,  5.5f,  0.0f),
		vec3( 0.0f,  5.5f,  0.0f),
		// test mouse
		vec3(0.0f,0.0f,-20.0f)
	};
	unsigned int VBO,VAO,lightVAO,planeVAO,animateVAO;
	// VAO for object
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(mycube_phong),mycube_phong,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	//VAO for light
	glGenVertexArrays(1,&lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//VAO for plane
	glGenVertexArrays(1,&planeVAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(myplane_phong),myplane_phong,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	// VAO for my first animation
	glGenVertexArrays(1,&animateVAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(animateVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(mycube_phong),mycube_phong,GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	realShader.use(); // put shader program before modify any uniform value
	glEnable(GL_DEPTH_TEST);

	printf("---------------------------------------------------------------------\n");
	printf("Now building this with seperate shader file.\n");
	printf("Using camera class for easy management.\n");
	printf("There are %i camera in this world\n",myCamera::getNoCam());
	printf("NOW you can walk (tutorial implementation) WASD !!!\n");
	printf("---------------------------------------------------------------------\n");
	
	cam1.makeMyProjection((float)MY_WIDTH,(float)MY_HEIGHT,55.0f);
	//cam1.CameraOn(realShader);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vec3 mypath = vec3(-3.0f,0.0f,0.0f);
	bool goback = false;
	while(!glfwWindowShouldClose(mywin)){ 
		processInput(mywin);
		walkAround(mywin);

		glClearColor(0.1f,0.1f,0.1f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		//set light shader
		mylight.use();
		mylight.setVec3("lightSource",1.0f,1.0f,1.0f);
		mylight.setTransform("projection",value_ptr(cam1.getPROJECT()));
		mylight.setTransform("view",value_ptr(cam1.getVIEW()));
		glBindVertexArray(lightVAO);

		//set single light source
		mat4 model = mat4(1.0f);
		vec3 lightCube = vec3(2.0f,3.0f,-3.0f);
		model = translate(model, lightCube);
		model = scale(model,vec3(0.8f));
		mylight.setTransform("model",value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Set object shader
		realShader.use();
		myBox.myactivate(0);
		myBox2.myactivate(1);
		//realShader.setVec3("objColor",0.27f,0.72f,1.0f);
		realShader.setInt("myMat.diffuse",0);
		realShader.setInt("myMat.specular",1);
		realShader.setFloat("myMat.shininess",8.0f);
		//instead of define light struct . i just edit lightColor to be little darker .
		realShader.setVec3("lightColor",0.6f,0.6f,0.6f);
		realShader.setVec3("lightPos",lightCube.x,lightCube.y,-lightCube.z); //light on z axis have to be reversed . it's my own bug .
		realShader.setVec3("viewPos",cam1.getPOS().x,cam1.getPOS().y,cam1.getPOS().z); //specular here look out of place . might need to look over about vector operation again.
		cam1.CameraOn(realShader);
		glBindVertexArray(VAO);

		for(int i =0;i<15;i++){
			mat4 model = mat4(1.0f);
            model = translate(model, allCube[i]);
            realShader.setTransform("model",value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//floor
		realShader.setFloat("myMat.shininess",64.0f);
		myPlane.myactivate(0);
		myPlane2.myactivate(1);
		glBindVertexArray(planeVAO);

		model = mat4(1.0f);
		model = scale(model,vec3(10.0f));
		model = translate(model,vec3(0.0f,-0.55f,0.0f));
		realShader.setTransform("model",value_ptr(model));
		glDrawArrays(GL_TRIANGLES,0,6);

		//box animation
		realShader.setFloat("myMat.shininess",64.0f);
		animateMe.myactivate(0);
		animateMe2.myactivate(1);
		glBindVertexArray(animateVAO);

		model = mat4(1.0f);
		model = translate(model,mypath);
		if(goback){
			if(mypath.z >= -4.0f) mypath.z -=0.001f;
			else goback = false;
		}
		else{
			if(mypath.z <= 4.0f) mypath.z +=0.001f;
			else goback = true;
		}
		realShader.setTransform("model",value_ptr(model));
		glDrawArrays(GL_TRIANGLES,0,36);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteVertexArrays(1,&planeVAO);
	glDeleteVertexArrays(1,&animateVAO);
	glDeleteVertexArrays(1,&lightVAO);
	glDeleteBuffers(1,&VBO);

	realShader.destroy();
	mylight.destroy();

	glfwTerminate();
	return 0;
}