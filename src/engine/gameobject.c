#include <smugstd.h>
#include <stdlib.h>

#include <graphics/drawable/drawable.h>
#include <graphics/graphics.h>
#include <physics/body.h>
#include <common/log.h>
#include <platform/platform.h>
#include <utils/point.h>

#include <engine/gameobject.h>

static void _invariant(GameObject* self)
{
    smug_assert(NULL != self);
}

void _Body_deleteVoid(void* body)
{
    Body_delete((Body*)body);
}

GameObject* GameObject_new(void)
{
    GameObject* go = (GameObject*)malloc(sizeof(GameObject));

    // go->x = 0.0f;
    // go->y = 0.0f;

    go->drawables = LinkedList_new();
    go->bodies = LinkedList_new();
    go->visible = TRUE;
    go->tag = NULL;
    go->position = Interpoint_new(Point_createFromXY(0, 0));

    return go;
}

void GameObject_delete(void* self)
{
    GameObject* go = (GameObject*)self;
    _invariant(go);

    LinkedList_deleteContents(go->drawables, Drawable_delete);
    LinkedList_deleteContents(go->bodies, _Body_deleteVoid);
    LinkedList_delete(go->drawables);
    LinkedList_delete(go->bodies);
    Interpoint_delete(go->position);

    free(go);
}

void GameObject_setPos(GameObject* self, float x, float y)
{
    _invariant(self);
    Interpoint_setTo(self->position, Point_createFromXY(x, y));
}

void GameObject_moveTo(GameObject* self, float x, float y)
{
    Interpoint_moveTo(self->position, Point_createFromXY(x, y));
}

void GameObject_commitPosition(GameObject* self)
{
    Interpoint_commit(self->position);
    for (Node* node = self->drawables->first; node != NULL; node = node->next)
    {
        Drawable_commitPosition((Drawable*)node->item);
    }
}

Vector GameObject_getPos(GameObject* self)
{
    _invariant(self);
    return Point_getVector(Interpoint_getPoint(self->position));
}

float GameObject_getX(GameObject* self)
{
    _invariant(self);
    return Point_getX(Interpoint_getPoint(self->position));
}

float GameObject_getY(GameObject* self)
{
    _invariant(self);
    return Point_getY(Interpoint_getPoint(self->position));
}

Point GameObject_getPosForDrawing(GameObject* self)
{
    return Interpoint_getInterpolated(self->position);
}

void GameObject_addDrawable(GameObject* self, Drawable* d)
{
    _invariant(self);

    // Node* node;
    // int i;

    // node = self->drawables->first;

    // See if there is a "hole" in the list to fill.
    // for (i = 0; i < LinkedList_length(self->drawables); i++)
    // {
        // if (node->item == NULL)
        // {
            // node->item = d;
            // break;
        // }
        // node = node->next;
    // }

    // if (i >= LinkedList_length(self->drawables))
    // {   // There was no hole.
        LinkedList_addLast(self->drawables, (void*)d);
    // }
    Drawable_setParent(d, self);

    d->parent = self;
    // Graphics_addDrawable(d);

    // return i;
}

// Drawable* GameObject_getDrawable(GameObject* self, int drawableIndex)
// {
    // Node* node;
    // int i;

    // node = self->drawables->first;
    // for (i = 0; i < drawableIndex; i++)
    // {
        // node = node->next;
        // smug_assert(node != NULL);
    // }
    // smug_assert(node->item != NULL);
    // return (Drawable*)node->item;
// }

void GameObject_removeDrawable(GameObject* self, struct Drawable* drawable)
{
    _invariant(self);

    // Node* node;

    if (!LinkedList_removeItem(self->drawables, drawable))
    {
        WARNING("Tried to remove a Drawable that wasn't in the GameObject.");
    }
    Drawable_removeParent(drawable);
    // for (node = self->drawables->first; node != NULL; node = node->next)
    // {
        // if ((Drawable*)node->item == drawable)
        // {
            // LinkedList_remove(self->drawables, node);
            // break;
        // }
    // }
}

void GameObject_setOpacity(GameObject* self, float opacity)
{
    Node* node;
    for (node = self->drawables->first; node != NULL; node = node->next)
    {
        Drawable_setOpacity((Drawable*)node->item, opacity);
    }
}

int GameObject_addBody(GameObject* self, Body* b)
{
    _invariant(self);

    LinkedList_addLast(self->bodies, (void*)b);
    return LinkedList_length(self->bodies) - 1;
}

/*
void GameObject_render(GameObject* obj)
{
    _invariant(obj);
    if (NULL != obj->drawable && obj->visible)
    {
        Drawable_render(obj->drawable, obj->x, obj->y);
    }
}*/
