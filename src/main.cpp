#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION //don't know why we need this before calling image loader library. probably VScode thing.
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <myShader/myshader.h>

#define MY_PATH "C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\"
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
void move_cube(GLFWwindow *window,Shader shader,mat4 v){
	if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS){
		v = translate(v,vec3(-2.0f,0.0f,0.0f));
		shader.setTransform("view",value_ptr(v));
	}
	else if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS){
		v = translate(v,vec3(2.0f,0.0f,0.0f));
		shader.setTransform("view",value_ptr(v));
	}
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
	//now building shader from newly created Shader class
	std::string shader_path = MY_PATH;
	std::string shader_path2 = MY_PATH;
	shader_path.append("src\\shader\\simple_3D_space.vert");
	shader_path2.append("src\\shader\\simple_3D_space.frag");
	const char* chr1,* chr2;
	chr1 = shader_path.c_str();
	chr2 = shader_path2.c_str();
	Shader simpleShader_GLM(chr1,chr2);

	// loading image texture
	unsigned int main_texture,decal_texture;
	int w,h,nr;
	std::string texture_path = MY_PATH;
	//main texture
	glGenTextures(1,&main_texture);
	glBindTexture(GL_TEXTURE_2D,main_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture_path.append("resource\\texture\\mytile.jpg");
	chr1 = texture_path.c_str();
	//stbi_set_flip_vertically_on_load(true);  //call this function to load image properly (not upside-down)
	unsigned char *texture_data = stbi_load(chr1,&w,&h,&nr,0);
	if(texture_data){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,texture_data); //.jpg,.jpeg use GL_RGB BUT .png use GL_RGBA coz it store different color value
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else printf("FAILED TO LOAD IMAGE TEXTURE\n");
	stbi_image_free(texture_data);

	//decal texture 

	glGenTextures(1,&decal_texture);
	glBindTexture(GL_TEXTURE_2D,decal_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture_path.clear();
	texture_path = MY_PATH;
	texture_path.append("resource\\texture\\doge.png");
	chr1 = texture_path.c_str();
	unsigned char *texture_data2 = stbi_load(chr1,&w,&h,&nr,0); //loading another texture for texture unit dsb
	if(texture_data2){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,texture_data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else printf("FAILED TO LOAD IMAGE TEXTURE\n");
	stbi_image_free(texture_data2);

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

	printf("Now building this with seperate shader file . more managment needed.\n");
	printf("Press UP for decal blend in . Press Down for decal blend out.\n");
	printf("Press LEFT to go left (static). Press RIGHT to go right (static).\n");

	simpleShader_GLM.use();
	simpleShader_GLM.setInt("myTexture1",0); //set texture unit for main
	simpleShader_GLM.setInt("myTexture2",1); //set texture unit for decal
	float rate = 0.01f;
	simpleShader_GLM.setFloat("blending_rate",rate);

	//using GLM to set model,view,projection vector
	mat4 model = mat4(1.0f);
	mat4 view = mat4(1.0f);
	mat4 project = mat4(1.0f);
	
	model = rotate(model,radians(30.0f),vec3(1.0f,0.0f,0.0f));
	view = translate(view,vec3(0.0f,0.3f,-5.0f));
	project = perspective(radians(45.0f),(float)MY_WIDTH /(float)MY_HEIGHT,0.1f,100.0f);

	simpleShader_GLM.setTransform("model",value_ptr(model));
	simpleShader_GLM.setTransform("view",value_ptr(view));
	simpleShader_GLM.setTransform("projection",value_ptr(project));

	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0); //texture unit. mutiple texture data in one common location
	glBindTexture(GL_TEXTURE_2D,main_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,decal_texture);

	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);
		rate = change_texture(mywin,simpleShader_GLM,rate);
		move_cube(mywin,simpleShader_GLM,view);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		model = rotate(model,0.0002f, vec3(0.0f,1.0f, 0.0f));
		simpleShader_GLM.setTransform("model",value_ptr(model));
		simpleShader_GLM.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,36);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	simpleShader_GLM.destroy();

	glfwTerminate();
	return 0;
}