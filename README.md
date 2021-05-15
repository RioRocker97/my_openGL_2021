**### I discontiuned this side-project coz' i dont know what to do next . maybe i'll come back to re-open this IF i have another passion PUSH###**
**Last followed Tutorial : [Depth Testing(around mid-March 2021)](https://learnopengl.com/Advanced-OpenGL/Depth-testing)**

# my own study and research on computer graphic(Low-level programming) 

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
- load complex model using Assimp 
- can now build 3d world !!!
- can load simple diffuse and spec map based on Model's uv map

## my 3D world progression

- 14/3/2021
    - First 3d world is created ! (still lag and didn't optimize code . maybe should try re-write model class?)
- 17/3/2021
    - two possible source of laggy rendering is : either my shader program suck or camera movement is using too much computation
    - try to use assimp as low as possible .(just use it to load vertex,normal,textcord)
    - now seperate texture from assimp 's built in texture loader (might need somewhere to store all those texture data in some other class)

### my own header

- mytexture.h for storing GLtexture (simple 2D image texture)
- mycamera.h for creating camera in 3d world
- myshader.h for shader program
- **mylight.h for defining light simulation along with shader (proposed)
- all essential already re-write in realShader
- model.h and mesh.h for loading model using assimp (no texture loading implmentation)
- played around assimp for understanding how material loading work  

- lots of GLSL in shader folder 
