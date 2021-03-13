# Another OpenGL repo for studying computer graphic

- Could be useful when i start building my own game engine
- might be a good start to look into OpenGL/ES/WebGL/Vulkan
- understand what behind Magnum Engine ever more.

## Completion List

- First Triangle 
- first pentagon (with pulsing Color !)
- Pulsing Pentagon (from each side)
- 2D textured
- 2D texture unit (mutiple texture in same texture location)(with key to change value !)
- simple rotate and scale using vector
- first 3D cube with static control (left,right)
- create camera (using model,view,projection matrix)
- using mouse and keyboard to move around (by doing some calculation with matrix and vector)
- let there be light !
- first implement of Phong's shader (still buggy . light is static )
- now with texture ! (Diffuse and Specular map)
- first animation ! (why will it "lag" whenever i changed shininess ?)
- first realistic light (directional and point light)
### Library used

- GLM , GLAD , GLFW 

### my own header

- mytexture.h for storing GLtexture (simple 2D image texture)
- mycamera.h for creating camera in 3d world
- myshader.h for shader program
- **mylight.h for defining light simulation along with shader (proposed)
- **will be built newer and better header for Shader program , Light, Texture loading at realShader

- lots of GLSL in shader folder 

## Workspace

- Moving all application to application.h for file management 
- for now , don't edit main.cpp more often