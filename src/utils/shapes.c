#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <utils/rectangle.h>
#include <utils/vector.h>
#include <utils/linkedlist.h>

#include <utils/shapes.h>

typedef struct Multipoint
{
    Vector* points;
    unsigned int nrPoints;
    unsigned int pointIndex;    /**<  */
} Multipoint;

Shape* Shape_new(void)
{
    Shape* shape = malloc(sizeof(Shape));

    shape->type = SHAPE_UNKNOWN;
    shape->data = NULL;

    return shape;
}

void Shape_delete(Shape* self)
{
    switch(self->type)
        {
        case SHAPE_RECTANGLE:
            Rectangle_delete((Rectangle*)self->data);
            break;
        case SHAPE_MULTIPOINT:
            free(((Multipoint*)self->data)->points);
            free((Multipoint*)self->data);
            break;
        case SHAPE_UNFINISHED:
            LinkedList_deleteContents((LinkedList*)self->data, Vector_delete2d);
            LinkedList_delete((LinkedList*)self->data);
            break;
        }

    free(self);
}

Shape* Shape_newFromRectangle(Rectangle rect)
{
    Shape* shape = Shape_new();
    Rectangle* new_rect = Rectangle_new();

    *new_rect = rect;

    shape->type = SHAPE_RECTANGLE;
    shape->data = (void*) new_rect;

    return shape;
}

Rectangle Shape_getAsRectangle(Shape* self)
{
    smug_assert(self->type == SHAPE_RECTANGLE);
    return *(Rectangle*)self->data;
}

Shape* Shape_newMultipoint(void)
{
    Shape* newShape = Shape_new();
    newShape->type = SHAPE_UNFINISHED;
    newShape->data = LinkedList_new();
    return newShape;
}

Shape* Shape_addPoint(Shape* self, Vector point)
{
    smug_assert(self != NULL);
    smug_assert(self->type == SHAPE_UNFINISHED);
    Vector* v = Vector_new2d(Vector_getX(&point), Vector_getY(&point));
    LinkedList_addLast((LinkedList*)self->data, v);
    return self;
}

Shape* Shape_finishMultipoint(Shape* self)
{
    smug_assert(self != NULL);
    smug_assert(self->type == SHAPE_UNFINISHED);
    LinkedList* list = (LinkedList*)self->data;
    int nrPoints = LinkedList_length(list);
    smug_assert(nrPoints > 0);
    Vector* points = (Vector*)malloc(sizeof(Vector) * nrPoints);
    int i = 0;
    for (Node* node = list->first; node != NULL; node = node->next)
    {
        points[i] = *(Vector*)node->item;
    }
    LinkedList_deleteContents(list, Vector_delete2d);
    LinkedList_delete(list);
    Multipoint* mp = (Multipoint*)malloc(sizeof(Multipoint));
    mp->nrPoints = nrPoints;
    mp->points = points;
    self->data = mp;
    self->type = SHAPE_MULTIPOINT;
    return self;
}

Vector Shape_getFirstPoint(Shape* self)
{
    smug_assert(self != NULL);
    smug_assert(self->type == SHAPE_MULTIPOINT);
    ((Multipoint*)self->data)->pointIndex = 0;
    return Shape_getNextPoint(self);
}

BOOL Shape_morePoints(Shape* self)
{
    smug_assert(self != NULL);
    smug_assert(self->type == SHAPE_MULTIPOINT);
    return ((Multipoint*)self->data)->pointIndex < ((Multipoint*)self->data)->nrPoints;
}

Vector Shape_getNextPoint(Shape* self)
{
    smug_assert(self != NULL);
    smug_assert(self->type == SHAPE_MULTIPOINT);
    smug_assert(Shape_morePoints(self));
    return ((Multipoint*)self->data)->points[((Multipoint*)self->data)->pointIndex++];
}

int Shape_getNrPoints(Shape* self)
{
    smug_assert(self != NULL);
    switch (self->type)
    {
        case SHAPE_RECTANGLE:
            return 4;
        case SHAPE_MULTIPOINT:
            return ((Multipoint*)self->data)->nrPoints;
        default:
            smug_assert(!"Bad shape type!");
            return 0;
    }
}
