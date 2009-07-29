#include "signal.h"
#include "test-utils/assert.h"
#include <GL/glfw.h>
#include <stdio.h>

int main()
{
   int signal;
   assertTrue(TRUE == glfwInit());
   
   assertTrue(TRUE == Signal_init());
   assertTrue(FALSE == Signal_check(0x01));
   Signal_send(0x01);
   assertTrue(TRUE == Signal_check(0x01));
   assertTrue(TRUE == Signal_check(0x01));
   
   Signal_send(0x02);
   assertTrue(TRUE == Signal_check(0x01));
   assertTrue(TRUE == Signal_check(0x02));
   assertTrue(FALSE == Signal_check(0x04));

   Signal_clear(0x02);
   assertTrue(TRUE == Signal_check(0x01));
   assertTrue(FALSE == Signal_check(0x02));

   Signal_terminate();
   glfwTerminate();

   return 0;
}
