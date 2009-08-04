#ifndef SMUG_PHYSICS_SHAPE_H
#define SMUG_PHYSICS_SHAPE_H

#include "linkedlist.h"

typedef struct Polygon
{
    LinkedList* vertices;
    LinkedList* edges;
    LinkedList* normals;
} Polygon;

Polygon* Polygon_new();

void Polygon_delete(Polygon* polygon);

Polygon* Polygon_createRectangle(float w, float h);

void Polygon_calculateEdges(Polygon* polygon);

void Polygon_calculateNormals(Polygon* polygon);

void Polygon_print(Polygon* polygon);

#endif // SMUG_PHYSICS_SHAPE_H
