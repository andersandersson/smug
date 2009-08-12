#include <GL/glfw.h>
#include "shape.h"
#include "common/common.h"
#include "test-utils/assert.h"
#include "graphics/graphics.h"
#include "graphics/color.h"
#include "engine/engine.h"
#include "physics/debug.h"

int main()
{
    glfwInit();
    Graphics_init(640, 480, FALSE);

    BOOL running = TRUE;

    Vertex* v = Vertex_new2f(0.0, 0.0);
    Vertex_insertAfter(v, Vertex_new2f(0.5, 0.5));
    Vertex_insertAfter(v->next, Vertex_new2f(0.2, 0.7));
    Vertex_insertAfter(v->next->next, Vertex_new2f(-0.5, 0.5));

    while(running)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();

            Physics_drawVertices(v, 1.0, 1.0, 0.0);

            glfwSwapBuffers();
            
            if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
                {
                    running = FALSE;
                }
        }

    Vertex_deleteAll(v);

    Graphics_terminate();
    glfwTerminate();
    
    return 0;
}
