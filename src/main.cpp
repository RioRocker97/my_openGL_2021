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

#define MY_PATH "C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\";

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
	shader_path.append("src\\shader\\simple_vec.vert");
	shader_path2.append("src\\shader\\simple_text.frag");
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

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	printf("Now building this with seperate shader file . more managment needed.\n");
	printf("Press UP for decal blend in . Press Down for decal blend out.");
	simpleShader_GLM.use();
	simpleShader_GLM.setInt("myTexture1",0); //set texture unit for main
	simpleShader_GLM.setInt("myTexture2",1); //set texture unit for decal
	float rate = 0.3f;
	simpleShader_GLM.setFloat("blending_rate",rate);

	//using GLM to rotate and scale one time
	using namespace glm;
	/*mat4 trans = mat4(1.0f);
	trans = rotate(trans,radians(90.0f),vec3(0.0,0.0,1.0));
	trans = scale(trans,vec3(1.5,1.0,1.0));

	simpleShader_GLM.setTransform("transform",value_ptr(trans));
	*/
	// using GLM to rotate constantly
	mat4 trans = mat4(1.0f);
	//trans = translate(trans,vec3(0.5f,0.5f,0.0f));
	//trans = rotate(trans,(float)glfwGetTime(),vec3(0.0f,0.0f,1.0f));
	float myrate = 0.0001;
	bool toggle = true;
	while(!glfwWindowShouldClose(mywin)){
		processInput(mywin);
		rate = change_texture(mywin,simpleShader_GLM,rate);

		glClearColor((float)42/255,(float)229/255,(float)217/255,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//GLM rotate constantly
		//float myrate = sin(glfwGetTime())/200.0;
		//printf("myrate now : %.1f\n",myrate);
		if(toggle && (int)glfwGetTime() % 5 == 0){
			trans = translate(trans,vec3(-myrate,-myrate,0.0f));
			toggle=false;
		}
		else{
			trans = translate(trans,vec3(myrate,myrate,0.0f));
			toggle=true;
		}
		//printf("myrate now : %.3f\n",myrate);
		trans = rotate(trans,0.0005f,vec3(0.0f,0.0f,1.0f));
		simpleShader_GLM.setTransform("transform",value_ptr(trans));
		//
		simpleShader_GLM.use();

		glActiveTexture(GL_TEXTURE0); //texture unit. mutiple texture data in one common location
		glBindTexture(GL_TEXTURE_2D,main_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,decal_texture);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT, 0);

    	glfwSwapBuffers(mywin);
    	glfwPollEvents();    
	}

    glDeleteVertexArrays(2,VAO);
    glDeleteBuffers(2,VBO);
    glDeleteBuffers(2,EBO);
	simpleShader_GLM.destroy();

	glfwTerminate();
	return 0;
}