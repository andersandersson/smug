/* -*- tab-width: 4 -*- */ 

#include "GL/glfw.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "signal.h"
#include "console.h"

int main(char argc, char* argv[])
{
   glfwInit();
   Graphics_Init(640, 480, 0);
   
   
   while( TRUE != Signal_Check(SIG_EXIT) )
   {
      glfwSwapBuffers();      
      
      if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
         {
            Signal_Send(SIG_EXIT);
         }
   }
   
   glfwTerminate();
   return 0;
}
