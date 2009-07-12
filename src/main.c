/* -*- tab-width: 4 -*- */ 

#include "GL/glfw.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "signal.h"
#include "platform/threads.h"

#include <stdio.h>

void test() {
   char *cmd;
   
   while(TRUE != Signal_Check(SIG_EXIT) )
      {
         fprintf(stderr, "> ");
         scanf("%s", cmd);
         Signal_Send(SIG_EXIT);
      }
}

int main(char argc, char* argv[])
{
   Thread * t;
   glfwInit();
   Graphics_Init(640, 480, 0);  
   
   t = Thread_New();
   Thread_Call(t, test, NULL);

   while( TRUE != Signal_Check(SIG_EXIT) )
   {
      glfwSwapBuffers();      
      
      if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
         {
            Signal_Send(SIG_EXIT);
         }
   }
   
   Thread_Join(t);
   Thread_Delete(t);
   glfwTerminate();
   return 0;
}
