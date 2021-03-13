#include <application.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_PATH std::filesystem::current_path().parent_path().u8string().c_str()
int main(){
	glfwInit();
	GLFWwindow* app = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"Chang's Sandbox Phase 2",NULL,NULL); //window area
	myApp(app,WIN_WIDTH,WIN_HEIGHT,WIN_PATH);
	return 0;
}