#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <utils/point.h>
#include <utils/vector.h>

#include <physics/body.h>

Body* Body_new(void)
{
    Body* body = malloc(sizeof(Body));

    body->type = 0;
    body->shape = NULL;
    body->position = Point_createFromXY(0.0, 0.0);
    body->new_position = Point_createFromXY(0.0, 0.0);
    body->movement = Vector_create2d(0.0, 0.0);

    return body;
}

void Body_delete(Body* body)
{
    free(body);
}

void Body_setShape(Body* body, Shape* shape)
{
    body->shape = shape;
}

void Body_setPosition(Body* body, float x, float y)
{
    Point_setX(&body->position, x);
    Point_setY(&body->position, y);
    Point_setX(&body->new_position, x);
    Point_setY(&body->new_position, y);
}

void Body_move(Body* body, float x, float y)
{
    Vector_setX(&body->movement, x);
    Vector_setY(&body->movement, y);
}

void Body_moveTo(Body* body, float x, float y)
{
    Point_setX(&body->new_position, x);
    Point_setY(&body->new_position, y);
}
