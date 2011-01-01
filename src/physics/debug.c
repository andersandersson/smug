#include "debug.h"

#include <platform/opengl/opengl.h>

#include "common/log.h"

#include "utils/rectangle.h"

static void Physics_drawRectangle(Rectangle* rectangle, Point offset, Color color)
{
    float x = Point_getX(&offset);
    float y = Point_getY(&offset);

    float r_x = Rectangle_getX(rectangle);
    float r_y = Rectangle_getY(rectangle);
    float r_w = Rectangle_getW(rectangle);
    float r_h = Rectangle_getH(rectangle);

    Physics_drawLine(Point_createFromXY(r_x+x, r_y+y), Point_createFromXY(r_x+r_w+x, r_y+y), color);
    Physics_drawLine(Point_createFromXY(r_x+r_w+x, r_y+y), Point_createFromXY(r_x+r_w+x, r_y+r_h+y), color);
    Physics_drawLine(Point_createFromXY(r_x+r_w+x, r_y+r_h+y), Point_createFromXY(r_x+x, r_y+r_h+y), color);
    Physics_drawLine(Point_createFromXY(r_x+x, r_y+r_h+y), Point_createFromXY(r_x+x, r_y+y), color);
}

void Physics_drawShape(Shape* shape, Point offset, Color color)
{
    switch(shape->type)
        {
        case SHAPE_RECTANGLE:
            Physics_drawRectangle(shape->data, offset, color);
            break;
        default:
            break;
        }
}

void Physics_drawLine(Point source, Point dest, Color color)
{
    float x1 = Point_getX(&source);
    float y1 = Point_getY(&source);
    float x2 = Point_getX(&dest);
    float y2 = Point_getY(&dest);

    glBegin(GL_LINES);
    glColor4f(Color_Rf(color), Color_Gf(color), Color_Bf(color), Color_Af(color));
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
}
