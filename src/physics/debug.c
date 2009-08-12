#include "debug.h"

#include <GL/glfw.h>

void Physics_drawLine(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    glBegin(GL_LINES);
    glColor3f(r, g, b);

    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    
    glEnd();
}

void Physics_drawVertices(Vertex* vertices, float r, float g, float b)
{
    Vertex* vertex = vertices;
    Vertex* prev = vertices;
    
    do
        {
            vertex = vertex->next;            
            
            Physics_drawLine(prev->x->x[0], prev->x->x[1], vertex->x->x[0], vertex->x->x[1], r, g, b);

            prev = vertex;
        } while(vertex != vertices);
}
