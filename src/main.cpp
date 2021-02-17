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

#define MY_WIDTH 1280
#define MY_HEIGHT 720

using namespace glm;

vec3 cameraPos   = vec3(5.0f, 0.0f,  -20.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, 1.0f);
vec3 cameraUp    = vec3(0.0f, 1.0f, 0.0f); //only adjust Y coz it UP

bool firstMouse = true;
float rotateY   = 90.0f;	// rotateY is initialized to -90.0 degrees since a rotateY of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float rotateZ =  0.0f;
float lastX =  1280.0/ 2.0;
float lastY =  720.0 / 2.0;
float fov   =  55.0f;

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
    float cameraSpeed = 0.001f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
		firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.8f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    rotateY += xoffset;
    rotateZ += yoffset;

    // make sure that when rotateZ is out of bounds, screen doesn't get flipped
    if (rotateZ > 89.0f)
        rotateZ = 89.0f;
    if (rotateZ < -89.0f)
        rotateZ = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(rotateY)) * cos(glm::radians(rotateZ));
    front.y = sin(glm::radians(rotateZ));
    front.z = cos(glm::radians(rotateZ)) * sin(glm::radians(rotateY));
    cameraFront = glm::normalize(front);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 55.0f)
        fov = 55.0f;
}
int main(){
	//initilize GLFW window with minimal openGL 3.0 version
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

	// mouse and scroll callback
	glfwSetFramebufferSizeCallback(mywin, framebuffer_size_callback);
    glfwSetCursorPosCallback(mywin, mouse_callback);
    glfwSetScrollCallback(mywin, scroll_callback);

	glfwSetInputMode(mywin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader simpleShader_GLM("simple_3D_space.vert","simple_3D_space.frag");
	// loading image texture using my own class

	Texture2D main_texture("mytile.jpg",false);
	Texture2D decal_texture("doge.png",true);
	Texture2D main2_texture("colorful.jpg",false);
	// real 3D cube
	float mycube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
		//H
		vec3(-2.0f,0.0f,0.0f),
		vec3(-2.0f,1.1f,0.0f),
		vec3(-2.0f,2.2f,0.0f),
		vec3(-2.0f,3.3f,0.0f),
		vec3(-2.0f,4.4f,0.0f),
		vec3(-2.0f,5.5f,0.0f),

		vec3(-3.1f,3.0f,0.0f),
		vec3(-4.2f,3.0f,0.0f),
		vec3(-5.2f,3.0f,0.0f),

		vec3(-6.3f,0.0f,0.0f),
		vec3(-6.3f,1.1f,0.0f),
		vec3(-6.3f,2.2f,0.0f),
		vec3(-6.3f,3.3f,0.0f),
		vec3(-6.3f,4.4f,0.0f),
		vec3(-6.3f,5.5f,0.0f),
		//A
		vec3(-7.5f,0.0f,0.0f),
		vec3(-7.5f,1.1f,0.0f),
		vec3(-7.5f,2.2f,0.0f),
		vec3(-7.5f,3.3f,0.0f),
		vec3(-7.5f,4.4f,0.0f),
		vec3(-7.5f,5.5f,0.0f),

		vec3(-8.6f,5.5f,0.0f),
		vec3(-9.7f,5.5f,0.0f),
		vec3(-10.8f,5.5f,0.0f),

		vec3(-8.6f,3.0f,0.0f),
		vec3(-9.7f,3.0f,0.0f),
		vec3(-10.8f,3.0f,0.0f),

		vec3(-11.9f,5.5f,0.0f),
		vec3(-11.9f,4.4f,0.0f),
		vec3(-11.9f,3.3f,0.0f),
		vec3(-11.9f,2.2f,0.0f),
		vec3(-11.9f,1.1f,0.0f),
		vec3(-11.9f,0.0f,0.0f),
		//N
		vec3(-14.0f,0.0f,0.0f),
		vec3(-14.0f,1.1f,0.0f),
		vec3(-14.0f,2.2f,0.0f),
		vec3(-14.0f,3.3f,0.0f),
		vec3(-14.0f,4.4f,0.0f),
		vec3(-14.0f,5.5f,0.0f),

		vec3(-15.1f,4.0f,0.0f),
		vec3(-16.2f,3.0f,0.0f),
		vec3(-17.3f,2.0f,0.0f),
		vec3(-18.4f,1.0f,0.0f),
		vec3(-19.5f,0.0f,0.0f),

		vec3(-20.6f,0.0f,0.0f),
		vec3(-20.6f,1.1f,0.0f),
		vec3(-20.6f,2.2f,0.0f),
		vec3(-20.6f,3.3f,0.0f),
		vec3(-20.6f,4.4f,0.0f),
		vec3(-20.6f,5.5f,0.0f),
		//G
		vec3(-22.2f,0.0f,0.0f),
		vec3(-22.2f,1.1f,0.0f),
		vec3(-22.2f,2.2f,0.0f),
		vec3(-22.2f,3.3f,0.0f),
		vec3(-22.2f,4.4f,0.0f),
		vec3(-22.2f,5.5f,0.0f),

		vec3(-23.3f,5.5f,0.0f),
		vec3(-24.4f,5.5f,0.0f),
		vec3(-25.5f,5.5f,0.0f),
		vec3(-26.6f,5.5f,0.0f),
		vec3(-27.7f,5.5f,0.0f),

		vec3(-23.3f,0.0f,0.0f),
		vec3(-24.4f,0.0f,0.0f),
		vec3(-25.5f,0.0f,0.0f),
		vec3(-26.6f,0.0f,0.0f),
		vec3(-27.7f,0.0f,0.0f),

		vec3(-27.7f,1.1f,0.0f),
		vec3(-27.7f,2.2f,0.0f),
		vec3(-27.7f,3.3f,0.0f),

		vec3(-26.6f,3.3f,0.0f),
		vec3(-25.5f,3.3f,0.0f),
		vec3(-24.4f,3.3f,0.0f),
		vec3(-24.4f,2.2f,0.0f),
		// test mouse
		vec3(0.0f,0.0f,-30.0f)
	};
	/* VBO = vertices Buffer Object . for storing vertice info to GPU buffer
	VA0 = vertics Array Object . for sotring vertice info newly created
	EBO =Elemental Buffer Object . for combining VBO into a new shape
	*/
	unsigned int VBO,VAO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(mycube),mycube,GL_STATIC_DRAW);


	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	simpleShader_GLM.use(); // put shader program before modify any uniform value

	// set texture 
	simpleShader_GLM.setInt("myTexture1",0);
	simpleShader_GLM.setInt("myTexture2",1);
	float rate = 0.01f;
	simpleShader_GLM.setFloat("blending_rate",rate);

	main_texture.myactivate(0);
	decal_texture.myactivate(1);

	//using GLM to set model,view,projection vector

	glEnable(GL_DEPTH_TEST);

	printf("---------------------------------------------------------------------\n");
	printf("Now building this with seperate shader file . more managment needed.\n");
	printf("Press UP for decal blend in . Press Down for decal blend out.\n");
	printf("Press E and Q to swap main texture\n");
	printf("NOW you can walk (tutorial implementation) WASD !!!\n");
	printf("---------------------------------------------------------------------\n");

	while(!glfwWindowShouldClose(mywin)){ 
		processInput(mywin);
		rate = change_texture(mywin,simpleShader_GLM,rate);
		setDiffuse(mywin,main2_texture);
		setDiffuse2(mywin,main_texture);
		walkAround(mywin);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		simpleShader_GLM.use();

		mat4 project = perspective(radians(fov),(float)MY_WIDTH /(float)MY_HEIGHT,0.1f,100.0f);
		simpleShader_GLM.setTransform("projection",value_ptr(project));
        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        simpleShader_GLM.setTransform("view",value_ptr(view));

		for(int i =0;i<88;i++){
			mat4 model = mat4(1.0f);
            model = translate(model, allCube[i]);
            simpleShader_GLM.setTransform("model",value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
		}

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	simpleShader_GLM.destroy();

	glfwTerminate();
	return 0;
}