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

//vec3 cameraPos   = vec3(5.0f, 0.0f,  -20.0f);
//vec3 cameraFront = vec3(0.0f, 0.0f, 1.0f);
//vec3 cameraUp    = vec3(0.0f, 1.0f, 0.0f); //only adjust Y coz it UP
//moving projection and view vector into camera class
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
float change_texture(GLFWwindow *window,Shader shader,float rate){
	if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS){
		if(rate <1.0f){
			rate+= 0.0001f; //system respond to key so fast that it have to change with very very small amount.
			shader.setFloat("blending_rate",rate);
		}
		else{
			rate = 1.0f;
			shader.setFloat("blending_rate",1.0f);
		}
	}
	if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS){
		if(rate >0.0f){
			rate-=0.0001f;
			shader.setFloat("blending_rate",rate);
		}
		else{
			rate = 0.0f;
			shader.setFloat("blending_rate",0.0f);
		}
	}
	return rate;
}
void setDiffuse(GLFWwindow *window,Texture2D text){
	if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)text.myactivate(0);
}
void setDiffuse2(GLFWwindow *window,Texture2D text){
	if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS)text.myactivate(0);
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

	GLFWwindow* mywin = glfwCreateWindow(MY_WIDTH,MY_HEIGHT,"CHANG window",NULL,NULL); //window area
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

	Shader realShader("real3D.vert","real3D.frag");
	Shader mylight("real3D.vert","real3D_light.frag");

	float mycube[] = {
		-0.5f, -0.5f, -0.5f, // 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,//  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, // 0.0f, 1.0f
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
	vec3 lightCube[] = {
		vec3(0.0f,1.0f,-5.0f),
		vec3(2.0f,3.0f,-5.0f),
	};
	unsigned int VBO,VAO,lightVAO;
	// VAO for object
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(mycube),mycube,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//VAO for light
	glGenVertexArrays(1,&lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

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

	while(!glfwWindowShouldClose(mywin)){ 
		processInput(mywin);
		walkAround(mywin);

		glClearColor(0.1f,0.1f,0.1f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// Set object shader
		realShader.use();
		realShader.setVec3("objColor",0.27f,0.72f,1.0f);
		realShader.setVec3("lightColor",1.0f,1.0f,1.0f);
		cam1.CameraOn(realShader);
		glBindVertexArray(VAO);
		for(int i =0;i<15;i++){
			mat4 model = mat4(1.0f);
            model = translate(model, allCube[i]);
            realShader.setTransform("model",value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//set light shader
		mylight.use();
		mylight.setVec3("lightSource",1.0f,1.0f,1.0f);
		mylight.setTransform("projection",value_ptr(cam1.getPROJECT()));
		mylight.setTransform("view",value_ptr(cam1.getVIEW()));
		glBindVertexArray(lightVAO);
		for(int i =0;i<2;i++){
			mat4 model = mat4(1.0f);
            model = translate(model, lightCube[i]);
			model = scale(model,vec3(0.5f));
            mylight.setTransform("model",value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
		}

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteVertexArrays(1,&lightVAO);
	glDeleteBuffers(1,&VBO);

	realShader.destroy();
	mylight.destroy();

	glfwTerminate();
	return 0;
}