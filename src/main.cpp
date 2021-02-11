#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION //don't know why we need this before calling image loader library. probably VScode thing.
#include <stb_image.h>

#include <myShader/myshader.h>

#define MY_PATH "C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\";

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)

{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
	std::string shader_path = MY_PATH;
	std::string shader_path2 = MY_PATH;
	shader_path.append("src\\shader\\simple_text.vert");
	shader_path2.append("src\\shader\\simple_text.frag");
	const char* chr1,* chr2;
	chr1 = shader_path.c_str();
	chr2 = shader_path2.c_str();
	Shader simpleShader_text(chr1,chr2);

	// loading image texture
	unsigned int face_texture;
	glGenTextures(1,&face_texture);
	glBindTexture(GL_TEXTURE_2D,face_texture);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w,h,nr;
	std::string texture_path = MY_PATH;
	texture_path.append("resource\\texture\\face.png");
	chr1 = texture_path.c_str();
	stbi_set_flip_vertically_on_load(true);  //call this function to load image properly (not upside-down)
	unsigned char *texture_data = stbi_load(chr1,&w,&h,&nr,0);
	if(texture_data){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,texture_data); //.jpg,.jpeg use GL_RGB BUT .png use GL_RGBA coz it store different color value
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else printf("FAILED TO LOAD IMAGE TEXTURE\n");
	stbi_image_free(texture_data);
	//

	//create pentagon
	float mycube[]={
		//position			//color				//texture coordinate
		-0.3f,-0.5f,0.0f,	1.0f,0.0f,0.0f,		0.3f,0.0f,
		-0.4f,0.0f,0.0f,	0.0f,1.0f,0.0f,		0.0f,0.5f,
		0.0f,0.5f,0.0f,		0.0f,0.0f,1.0f,		0.5f,1.0f,
		0.4f,0.0f,0.0f,		0.0f,1.0f,0.0f,		1.0f,0.5f,
		0.3f,-0.5f,0.0f,	1.0f,0.0f,0.0f,		0.7f,0.0f,
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
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//VAO for color value
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//VAO for texture cord. value
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0); 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	printf("Now building this with seperate shader file . more managment needed.");
	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		simpleShader_text.use();

		glBindTexture(GL_TEXTURE_2D,face_texture);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}

    glDeleteVertexArrays(2,VAO);
    glDeleteBuffers(2,VBO);
    glDeleteBuffers(2,EBO);
	simpleShader_text.destroy();

	glfwTerminate();
	return 0;
}