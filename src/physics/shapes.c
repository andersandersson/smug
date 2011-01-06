#include <stdlib.h>

#include <common/common.h>
#include <common/log.h>
#include <utils/rectangle.h>

#include <physics/shapes.h>

Shape* Shape_new(void)
{
    Shape* shape = malloc(sizeof(Shape));

    shape->type = SHAPE_UNKNOWN;
    shape->data = NULL;

    return shape;
}

void Shape_delete(Shape* shape)
{
    switch(shape->type)
        {
        case SHAPE_RECTANGLE:
            Rectangle_delete((Rectangle*)shape->data);
            break;
        }

    free(shape);
}

Shape* Shape_createFromRectangle(Rectangle rect)
{
    Shape* shape = Shape_new();
    Rectangle* new_rect = Rectangle_new();

    *new_rect = rect;

    shape->type = SHAPE_RECTANGLE;
    shape->data = (void*) new_rect;

    return shape;
}
