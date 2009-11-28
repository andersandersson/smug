#ifndef SMUG_PHYSICS_SHAPES_H
#define SMUG_PHYSICS_SHAPES_H


#include "utils/rectangle.h"
#include "utils/point.h"
#include "utils/vector.h"


typedef unsigned int SHAPE_TYPE;

#define SHAPE_UNKNOWN   0x00
#define SHAPE_RECTANGLE 0x01

typedef struct Shape
{
    SHAPE_TYPE type;
    void* data;         
} Shape;



Shape* Shape_new();
void Shape_delete(Shape* shape);

Shape* Shape_createFromRectangle(Rectangle rect);


#endif // SMUG_PHYSICS_SHAPES_H
