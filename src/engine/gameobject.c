#include "gameobject.h"
#include <stdlib.h>

#include "graphics/drawable/drawable.h"
#include "graphics/graphics.h"
#include "physics/body.h"
#include "common/log.h"

static void _invariant(GameObject* self)
{
    assert(NULL != self);
}

GameObject* GameObject_new()
{
    GameObject* go = (GameObject*)malloc(sizeof(GameObject));

    // go->x = 0.0f;
    // go->y = 0.0f;

    go->drawables = LinkedList_new();
    go->bodies = LinkedList_new();
    go->visible = TRUE;
    go->tag = NULL;

    return go;
}

void GameObject_delete(void* self)
{
    GameObject* go = (GameObject*)self;
    _invariant(go);

    LinkedList_deleteContents(go->drawables, Drawable_delete);
    LinkedList_deleteContents(go->bodies, Body_delete);
    LinkedList_delete(go->drawables);
    LinkedList_delete(go->bodies);

    free(go);
}

void GameObject_setPos(GameObject* self, float x, float y)
{
    _invariant(self);
    self->position = Vector_create2d(x, y);
    // Set position of all bodies and drawables.
    Node* node;

    for (node = self->drawables->first; node != NULL; node = node->next)
    {
        if (node->item != NULL)
            Drawable_updatePos((Drawable*)node->item);
    }

    for (node = self->bodies->first; node != NULL; node = node->next)
    {
        if (node->item != NULL)
        {
            // Body_updatePos((Body*)node->item);
        }
    }
}

Vector GameObject_getPos(GameObject* self)
{
    _invariant(self);
    return self->position;
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
        // assert(node != NULL);
    // }
    // assert(node->item != NULL);
    // return (Drawable*)node->item;
// }

void GameObject_removeDrawable(GameObject* self, struct Drawable* drawable)
{
    _invariant(self);

    // Node* node;

    if (!LinkedList_removeItem(self->drawables, drawable))
        WARNING("Tried to remove a Drawable that wasn't in the GameObject.");
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
