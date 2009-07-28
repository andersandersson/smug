#include <stdio.h>

#include "engine/engine.h"

#include "graphics/drawable.h"
#include "graphics/graphics.h"
#include "graphics/color.h"

#include "utils/rectangle.h"

int main()
{
    Rectangle* rect;
    GameObject* go;
    Drawable* drawable;
    Color color;
    
    Engine_init();
    
    Rectangle* area;
    area = Rectangle_new(0.0, 0.0, 640.0, 480.0);
    Graphics_setVisibleArea(area);
    Rectangle_delete(area);
    
    go = GameObject_new();
    rect = Rectangle_new(0.0f, 0.0f, 100.0f, 100.0f);

    //color = Color_new(1.0f, 1.0f, 0.0f, 1.0f);
    //drawable = Drawable_newRect(rect, color);
    //Color_delete(color);
    color = Color_createFromRgba(1.0f, 1.0f, 0.0f, 1.0f);
    drawable = Drawable_newRect(rect, &color);
    
    GameObject_setDrawable(go, drawable);
    GameObject_setPosition(go, 50.0f, 50.0f);
    Engine_addObject(go);
    
    Engine_run();

    Engine_terminate();

    GameObject_delete(go);
    
    return 0;
}
