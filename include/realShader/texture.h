#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

class Texture{
    private:
        unsigned int ID;
        string type;
        string path;
    public:
        //only load image texture in .JPG format
        //all image texture should be in /resource/texture
        Texture(const char* main_path,string filename){
            glGenTextures(1,&ID);
            glBindTexture(GL_TEXTURE_2D,ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_set_flip_vertically_on_load(true);  
            
            string full_path = main_path;
            full_path.append(filename);
            int w,h,nr;
            unsigned char* texture_data = stbi_load(full_path.c_str(),&w,&h,&nr,0);

            if(texture_data){
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,texture_data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            else printf("FAILED TO LOAD IMAGE TEXTURE >> %s \n",filename);
	        stbi_image_free(texture_data);
        }
        
        void use(int count){
            glActiveTexture(GL_TEXTURE0+count);
	        glBindTexture(GL_TEXTURE_2D,ID);
        }
        static void reset(){
            glActiveTexture(GL_TEXTURE0);
        }
};

#endif