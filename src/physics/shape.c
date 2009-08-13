#include "shape.h"

#include <stdlib.h>

Vertex* Vertex_new()
{
    return Vertex_new2f(0.0, 0.0);
}

Vertex* Vertex_new2f(float x, float y)
{
    Vertex* vertex = malloc(sizeof(Vertex));
    
    vertex->next = vertex;
    vertex->prev = vertex;
    
    vertex->x = Vector_new2f(x, y);

    return vertex;
}

void Vertex_insertAfter(Vertex* dest, Vertex* v)
{
    v->prev = dest;
    v->next = dest->next;
    dest->next = v;
}

void Vertex_delete(Vertex* vertex)
{
    if(NULL != vertex)
        {
            Vector_delete(vertex->x);
            free(vertex);
        }
}

void Vertex_deleteAll(Vertex* vertices)
{
    if(NULL != vertices)
        {
            Vertex* vertex = vertices;
            Vertex* node;
            
            do
                {
                    node = vertex;
                    vertex = vertex->next;            
                    Vertex_delete(node);
                } while(vertex != vertices);
        }
}

Polygon* Polygon_new()
{
    Polygon* polygon;

    polygon = malloc(sizeof(Polygon));
    polygon->next = NULL;
    polygon->prev = NULL;
    polygon->vertices = NULL;
}

void Polygon_delete(Polygon* polygon)
{
    Vertex_deleteAll(polygon->vertices);
    
    if(NULL != polygon)
        {
            free(polygon);
        }
}


Shape* Shape_new()
{
    
}

Shape* Shape_newRectangle(float w, float h)
{
}

void Shape_delete(Shape* shape)
{
}
