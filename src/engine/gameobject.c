#include "gameobject.h"
#include <stdlib.h>

#include "graphics/drawable/drawable.h"

static void GameObject_invariant(GameObject* go)
{
    assert(NULL != go);
}

GameObject* GameObject_new()
{
    GameObject* go = (GameObject*)malloc(sizeof(GameObject));
    
    // go->x = 0.0f;
    // go->y = 0.0f;
    
    // go->shape = NULL;
    go->drawable = NULL;
    go->visible = TRUE;
    go->tag = NULL;
    
    return go;
}

void GameObject_delete(void* obj)
{
    GameObject* go = (GameObject*)obj;
    GameObject_invariant(go);
    
//    if (go->shape)
        // Delete shape.
        
    if (NULL != go->drawable)
        Drawable_delete((void*)go->drawable);
        
    //if (go->tag) // What do to with the char*?
    
    free(go);
}

void GameObject_setPosition(GameObject* obj, float x, float y)
{
    GameObject_invariant(obj);
    // obj->x = x;
    // obj->y = y;
}

void GameObject_setDrawable(GameObject* obj, Drawable* d)
{
    GameObject_invariant(obj);
//    assert(NULL != d);
    
    obj->drawable = d;
}

/*
void GameObject_render(GameObject* obj)
{
    GameObject_invariant(obj);
    if (NULL != obj->drawable && obj->visible)
    {
        Drawable_render(obj->drawable, obj->x, obj->y);
    }
}*/
