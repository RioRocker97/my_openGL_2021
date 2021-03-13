#ifndef SINGLECAMERA_H
#define SINGLECAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include <realShader/shader.h>

using namespace glm;

class SingleCamera {
    private:
        vec3 POS;
        vec3 FRONT;
        vec3 UP;
        
        mat4 VIEW;
        mat4 PROJECTION;

        float width;
        float height;
        bool firstMouse = true;
        float yaw ;
        float pitch;
        float lastX;
        float lastY;
        float fov;

    public:
        static int id ;
        SingleCamera(float posX, float posY,float posZ){
            id++;
            POS = vec3(posX,posY,posZ);
            FRONT =vec3(0.0f,0.0f,1.0f);
            UP = vec3(0.0f,1.0f,0.0f);

            VIEW = lookAt(POS,POS+FRONT,UP);
        }
        void makeMyProjection(float w,float h,float FOV){
            width = w;
            height = h;
            fov = FOV;
            yaw = 90.0f;
            pitch = 0.0f;
            lastX = w/2.0;
            lastY = h/2.0;

            PROJECTION = perspective(radians(FOV),w/h,1.0f,100.0f);
        }
        void move(float posX,float posY,float cameraSens){
            if (firstMouse){
                lastX = posX;
                lastY = posY;
                firstMouse = false;
            }
            float xoffset = posX - lastX;
            float yoffset = lastY - posY;
            lastX = posX;
            lastY = posY;

            xoffset *= cameraSens;
            yoffset *= cameraSens;

            yaw += xoffset;
            pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            vec3 front;
            front.x = cos(radians(yaw)) * cos(radians(pitch));
            front.y = sin(radians(pitch));
            front.z = cos(radians(pitch)) * sin(radians(yaw));
            FRONT = normalize(front);

            VIEW = lookAt(POS,POS+FRONT,UP);

        }
        static int getNoCam(){return id;}
        void CameraOn(Shader shade){
            VIEW = lookAt(POS,POS+FRONT,UP);
            PROJECTION = perspective(radians(fov),width/height,1.0f,100.0f);
            shade.setTransform("projection",value_ptr(PROJECTION));
            shade.setTransform("view",value_ptr(VIEW));
        }
        vec3 getPOS(){return POS;}
        vec3 getFRONT(){return FRONT;}
        vec3 getUP(){return UP;}
        mat4 getVIEW(){return VIEW;}
        mat4 getPROJECT(){return PROJECTION;}
        float getFOV(){return fov;}
        void setPOS(vec3 x){POS = x;}
        void setFRONT(vec3 x){FRONT = x;}
        void setUP(vec3 x){UP = x;}
        void setFOV(float x){
            fov = x;
            if (fov < 1.0f) fov = 1.0f;
            else if (fov > 55.0f) fov = 55.0f;
            }
};

#endif