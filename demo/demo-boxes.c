#include <stdio.h>

#include "graphics/graphics.h"
#include "graphics/renderer/renderer.h"
#include "graphics/drawable/drawable.h"
#include "graphics/drawable/box.h"*/

#include "common/common.h"
#include "platform/platform.h"
//#include "platform/opengl/opengl.h"

#include "common/log.h"

int main()
{
    Log_init();

    if (!Platform_init())
        return 0;
    
    Log_print("Initializing\n");

    Platform_openWindow(640, 480, FALSE);    

    if (!Graphics_init(640, 480))
        return 0;
        
   
    Box* box1 = Box_newFromData(50, 50, 25, 75);
    Box* box2 = Box_newFromData(100, 100, 75, 25);
    
    box1->color = Color_createFromRGBA(1, 0.5, 0.5, 1);
    box2->color = Color_createFromRGBA(0.5, 1, 0.5, 1);  
    

    Graphics_addDrawable((Drawable*)box1);
    Graphics_addDrawable((Drawable*)box2);
    
   
    Log_print("Running\n");
    while (!Platform_getKey(KEY_ESC))
    {
        Graphics_render();   
 
        Platform_refreshWindow();
        Platform_sleep(0.1);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");
    Drawable_delete(box1);
    Drawable_delete(box2);

    Graphics_terminate();
    
    Platform_closeWindow();   
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();

    return 0;
}
