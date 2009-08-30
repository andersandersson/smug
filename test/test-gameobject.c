#include <stdio.h>

#include "engine/gameobject.h"

#include "graphics/drawable/drawable.h"
#include "graphics/color.h"

#include "utils/rectangle.h"

int main()
{
    Rectangle* rect;
    GameObject* go;
    Drawable* drawable;
    Color* color;
    
    go = GameObject_new();
    rect = Rectangle_new(0.0f, 0.0f, 100.0f, 100.0f);

    color = Color_new(1.0f, 1.0f, 0.0f, 1.0f);
    drawable = Drawable_newRect(rect, color);
    //color = Color_createFromRgba(1.0f, 1.0f, 0.0f, 1.0f);
    //drawable = Drawable_newRect(rect, &color);

    GameObject_setDrawable(go, drawable);
    GameObject_setPosition(go, 50.0f, 50.0f);

    GameObject_delete(go);
    
    return 0;
}
