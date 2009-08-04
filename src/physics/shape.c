#include "shape.h"

#include <stdlib.h>
#include <stdio.h>

Polygon* Polygon_new()
{
    Polygon* polygon;

    polygon = malloc(sizeof(Polygon));

    polygon->vertices = LinkedList_new();
    polygon->edges = LinkedList_new();
    polygon->normals = LinkedList_new();

    return polygon;
}

void Polygon_delete(Polygon* polygon)
{
    free(polygon->vertices);
    free(polygon->edges);
    free(polygon->normals);
    free(polygon);
}

Polygon* Polygon_createRectangle(float w, float h)
{
    Polygon* polygon;

    polygon = Polygon_new();

    return polygon;
}

void Polygon_calculateEdges(Polygon* polygon)
{
}

void Polygon_calculateNormals(Polygon* polygon)
{
}

void Polygon_print(Polygon* polygon)
{
    
}
