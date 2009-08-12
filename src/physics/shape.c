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
    Vector_delete(vertex->x);
    free(vertex);
}

void Vertex_deleteAll(Vertex* vertices)
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
