#ifndef SMUG_PHYSICS_DEBUG_H
#define SMUG_PHYSICS_DEBUG_H

#include <physics/shapes.h>
#include <utils/point_type.h>
#include <graphics/color_type.h>

void Physics_drawShape(Shape* shape, Point offset, Color color);
void Physics_drawLine(Point source, Point dest, Color color);

#endif // SMUG_PHYSICS_DEBUG_H
