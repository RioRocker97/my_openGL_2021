#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

#define TEXTURE_PATH "C:\\Users\\ChangNoi_V2\\Desktop\\Covid-19 shit\\openGL_again\\main_app\\resource\\texture\\"

class Texture2D{
    private:
        unsigned int ID;
    public:
        Texture2D(string filename,bool isPNG){
            glGenTextures(1,&ID);
            glBindTexture(GL_TEXTURE_2D,ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            string full_path = TEXTURE_PATH;
            full_path.append(filename);
            int w,h,nr;
            unsigned char* texture_data = stbi_load(full_path.c_str(),&w,&h,&nr,0);

            if(texture_data){
                if(isPNG){
                    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,texture_data);
		            glGenerateMipmap(GL_TEXTURE_2D);
                }
                else{
                    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,texture_data);
		            glGenerateMipmap(GL_TEXTURE_2D);
                }
            }

            else printf("FAILED TO LOAD IMAGE TEXTURE >> %s \n",filename);
	        stbi_image_free(texture_data);
        }
        
        void myactivate(int count){
            glActiveTexture(GL_TEXTURE0+count);
	        glBindTexture(GL_TEXTURE_2D,ID);
        }
};

#endif