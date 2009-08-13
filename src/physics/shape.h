#ifndef SMUG_PHYSICS_SHAPE_H
#define SMUG_PHYSICS_SHAPE_H

#include "vector.h"

typedef struct Vertex
{
    struct Vertex* next;
    struct Vertex* prev;    
    Vector* x;
} Vertex;

typedef struct Polygon
{
    struct Polygon* next;
    struct Polygon* prev;
    Vertex* vertices;
} Polygon;

typedef struct Shape
{
    unsigned int type;
    Polygon* polygons;
} Shape;

Vertex* Vertex_new();
Vertex* Vertex_new2f(float x, float y);
void Vertex_insertAfter(Vertex* dest, Vertex* v);
void Vertex_delete(Vertex* vertex);
void Vertex_deleteAll(Vertex* vertices);

Polygon* Polygon_new();
void Polygon_delete(Polygon* polygon);

Shape* Shape_new();
Shape* Shape_newRectangle(float w, float h);
void Shape_delete(Shape* shape);

#endif // SMUG_PHYSICS_SHAPE_H
