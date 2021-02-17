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
int main(){
	//initilize GLFW window with minimal openGL 3.0 version
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mywin = glfwCreateWindow(MY_WIDTH,MY_HEIGHT,"GLFW window",NULL,NULL); //window area
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

	glViewport(0,0,MY_WIDTH,MY_HEIGHT); //render area
	glfwSetFramebufferSizeCallback(mywin, framebuffer_size_callback);
	//now building shader from newly created Shader class (with path defined)
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
	mat4 project;
	project = perspective(radians(55.0f),(float)MY_WIDTH /(float)MY_HEIGHT,0.1f,100.0f);
	simpleShader_GLM.setTransform("projection",value_ptr(project));

	glEnable(GL_DEPTH_TEST);

	printf("---------------------------------------------------------------------\n");
	printf("Now building this with seperate shader file . more managment needed.\n");
	printf("Press UP for decal blend in . Press Down for decal blend out.\n");
	printf("Press E and Q to swap main texture\n");
	printf("---------------------------------------------------------------------\n");

	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);
		rate = change_texture(mywin,simpleShader_GLM,rate);
		setDiffuse(mywin,main2_texture);
		setDiffuse2(mywin,main_texture);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		simpleShader_GLM.use();

        mat4 view = mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float radius = 25.0f;
        float camX   = sin(glfwGetTime()/2.0) * radius ;
        float camZ   = cos(glfwGetTime()/2.0) * radius ;
        view = lookAt(vec3(camX,5.0f,camZ),vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 3.0f, 0.0f));
		//view = rotate(view,radians(-180.0f),vec3(0.0f,1.0f,0.0f));
        simpleShader_GLM.setTransform("view",value_ptr(view));

		for(int i =0;i<87;i++){
			mat4 model = mat4(1.0f);
            model = translate(model, allCube[i]);
			model = translate(model,vec3(10.0f,-1.0f,0.0f));
			//model = rotate(model,radians(20.0f),vec3(1.0f,0.0f,0.0f));
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