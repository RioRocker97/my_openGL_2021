#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)

{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//simple GLSL for define position and diffuse color (fragment shader)
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.63f,0.08f,0.11f,1.0f);\n"
	"}\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.54f,0.08f,0.73f,1.0f);\n"
	"}\0";
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

	//create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);

	//check vertexShader compiler 
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",infoLog);
	}
	//create fragment shader = fill shape with color
	unsigned int fragmentShader,fragmentShader_purple;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader_purple = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glShaderSource(fragmentShader_purple, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);
	glCompileShader(fragmentShader_purple);		
	//create shader program for binding vertex and fragment shader
	//there 2 fragment shader here . red and purple one
	unsigned int shaderProgram = glCreateProgram();
	unsigned int shaderProgram_purple = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glAttachShader(shaderProgram_purple, vertexShader);
	glAttachShader(shaderProgram_purple, fragmentShader_purple);
	glLinkProgram(shaderProgram_purple);

	//check Link Program compiler
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s",infoLog);
	}
	glGetProgramiv(shaderProgram_purple, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram_purple, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT_ORANGE::COMPILATION_FAILED\n%s",infoLog);
	}
	//shader program linking is completed so delete shader for performance
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader_purple);

	//create 2 triangle to form a square . red one
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
    	-0.5f,0.5f,0.0f,
       	0.5f,0.5f,0.0f,
       	0.5f,-0.5f,0.0f,
	};
	unsigned int combine[] ={
		0,1,2,
		0,2,3,
	};
	//create 2 triangle to form a square . purple one
	float vertices2[] = {
		-0.3f,-0.6f,0.0f,
		-0.3f,-0.9f,0.0f,
		0.3f,-0.6f,0.0f,
		0.3f,-0.9f,0.0f, 
	};
	unsigned int combine2[] ={
		0,1,2,
		1,2,3
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
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//bind combine vertexs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(combine),combine,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	//another rectangle with different color
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(combine2),combine2,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0); 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // for drawing polygon line of the objects

	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_TRIANGLES,0,6);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram_purple);
		glBindVertexArray(VAO[1]);
		//glDrawArrays(GL_TRIANGLES,0,6);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}

    glDeleteVertexArrays(2,VAO);
    glDeleteBuffers(2,VBO);
    glDeleteBuffers(2,EBO);
    glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram_purple);

	glfwTerminate();
	return 0;
}