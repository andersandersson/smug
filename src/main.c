#include "GL/glfw.h"
#include "graphics/graphics.h"
#include "physics/physics.h"

int main(char argc, char* argv[])
{
  char running = 0;

  glfwInit();

  Physics_Init();
  Graphics_Init(640, 480, 0);

  do
    {
      glfwSwapBuffers();      

      running = !glfwGetKey( GLFW_KEY_ESC ) &&
	glfwGetWindowParam( GLFW_OPENED );
    }
  while( running );
  
  return 0;
}
