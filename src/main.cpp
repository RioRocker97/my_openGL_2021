#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include <myShader/myshader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)

{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//simple GLSL for define position and diffuse color (fragment shader)
//can specifiy variable for output to be used for other shader source 
// now moving shaderSource into seperate file

int main(){
	//initilize GLFW window with minimal openGL 3.0 version
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mywin = glfwCreateWindow(800,600,"GLFW window",NULL,NULL); //window area
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

	glViewport(0,0,800,600); //render area
	glfwSetFramebufferSizeCallback(mywin, framebuffer_size_callback);
	//now building shader from newly created Shader class
	Shader simpleShader(
		"C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\src\\shader\\simple.vert"
		,"C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\src\\shader\\simple.frag");
	//create pentagon
	float mycube[]={
		//position			//color
		-0.3f,-0.5f,0.0f,	1.0f,0.0f,0.0f,
		-0.4f,0.0f,0.0f,	0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,		0.0f,0.0f,1.0f,
		0.4f,0.0f,0.0f,		0.0f,1.0f,0.0f,
		0.3f,-0.5f,0.0f,	1.0f,0.0f,0.0f
	};
	unsigned int bundle[]={
		0,1,2,
		0,2,4,
		2,3,4,
	};
	/* VBO = vertices Buffer Object . for storing vertice info to GPU buffer
	VA0 = vertics Array Object . for sotring vertice info newly created
	EBO =Elemental Buffer Object . for combining VBO into a new shape
	*/
	unsigned int VBO[2],VAO[2],EBO[2];
	glGenVertexArrays(2,VAO);
	glGenBuffers(2,VBO);
	glGenBuffers(2,EBO);

	glBindVertexArray(VAO[0]);

	//bind all vertexs
	glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(mycube),mycube,GL_DYNAMIC_DRAW); //telling openGL that there will be VAO changed by passing GL_DYNAMIC_DRAW
	//bind combine vertexs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(bundle),bundle,GL_STATIC_DRAW);

	//VAO for postion value
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//VAO for color value
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0); 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // for drawing polygon line of the objects
	printf("Now building this with seperate shader file . more managment needed.");
	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		simpleShader.use();

		///try to cycle color for each verticle success !!
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		mycube[4] = greenValue;
		mycube[10] = greenValue;
		mycube[16] = greenValue;
		mycube[22] = greenValue;
		mycube[28] = greenValue;
		glBufferData(GL_ARRAY_BUFFER,sizeof(mycube),mycube,GL_DYNAMIC_DRAW);
		///
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}

    glDeleteVertexArrays(2,VAO);
    glDeleteBuffers(2,VBO);
    glDeleteBuffers(2,EBO);

	glfwTerminate();
	return 0;
}