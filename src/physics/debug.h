#ifndef SMUG_PHYSICS_DEBUG_H
#define SMUG_PHYSICS_DEBUG_H

#include "shape.h"

void Physics_drawLine(float x1, float y1, float x2, float y2, float r, float g, float b);

void Physics_drawVertices(Vertex* vertices, Vector* offset, float r, float g, float b);
void Physics_drawPolygons(Polygon* polygons, Vector* offset, float r, float g, float b);
void Physics_drawShape(Shape* shape, Vector* offset, float r, float g, float b);

#endif // SMUG_PHYSICS_DEBUG_H
