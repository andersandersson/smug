
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics/graphics.h"
#include "graphics/renderer/renderer.h"
#include "graphics/drawable/drawable.h"
#include "graphics/drawable/box.h"


#include "common/common.h"
#include "platform/platform.h"
//#include "platform/opengl/opengl.h"

#include "common/log.h"

#define SWIDTH 640
#define SHEIGHT 480

float myRandom(float f)
{
    return f * ((float)rand() / (float)RAND_MAX);
}


int main()
{
    Log_init();

    if (!Platform_init())
        return 0;
    
    Log_print("Initializing\n");

    Platform_openWindow(640, 480, FALSE);    

    if (!Graphics_init(640, 480))
        return 0;
     
    LinkedList* objects = LinkedList_new();  
     
    int i = 0;
    for (i = 0; i < 5000; i++)
    {
        float r = myRandom(100);
        Box* box = Box_newFromPointRectangle(Point_createFromXY((SWIDTH * myRandom(3)) * cosf(r) + SWIDTH / 2, (SHEIGHT * myRandom(3)) * sinf(r) + SHEIGHT / 2), 
                                            Rectangle_createFromXYWH(myRandom(-15), myRandom(-15), myRandom(30), myRandom(30)));
        Graphics_addDrawable((Drawable*)box);
        LinkedList_addLast(objects, box);
    }
     
    Point target;
    
    Log_print("Running\n");
    TIME t;
    float dir = 1;
    float deltadir = 0.002;
    float color = 0;
    while (!Platform_getKey(KEY_ESC))
    {   
        t = Platform_getTime();
        target = Point_createFromXY((SWIDTH / 2) * cosf(t*2) + SWIDTH / 2, (SHEIGHT / 2) * sinf(t*2) + SHEIGHT / 2);
    
        Node* p = objects->first;
        while (p != NULL)
        {
            ((Drawable*)p->item)->pos = Point_addVector(((Drawable*)p->item)->pos, 
                                                            Vector_multiply(Point_distanceToPoint(((Drawable*)p->item)->pos, target), dir * (0.003 + myRandom(0.009))));
            ((Box*)p->item)->color = Color_createFromRGBA(0.5+sinf((color*3+0)*1.3)*0.5, 0.5+sinf((color*3+1)*1.3)*0.5, 0.5+sinf((color*3+2)*1.3)*0.5, 0.33);         
            p = p->next;
        }
 /*
        fprintf(stderr, "was: r:%i, g:%i, b:%i\n", (int)(128+sinf((color*3+0)*1.3)*128), (int)(128+sinf((color*3+1)*1.3)*128), (int)(128+sinf((color*3+2)*1.3)*128), 0.01);
        p = objects->first;
        fprintf(stderr, "is:  r:%i, g:%i, b:%i\n", (int)(((Box*)p->item)->color.r * 255.0f), (int)(((Box*)p->item)->color.g * 255.0f), (int)(((Box*)p->item)->color.b * 255.0f));
*/
  
        dir += deltadir;
        if (dir < -0.7)
        {
            dir = -0.7;
            deltadir = deltadir * -1;
        }
        if (dir > 1)
        {
            dir = 1;
            deltadir = deltadir * -1;
        }
        
        color+= 0.005;
        if (color > 24)
        {
            color -= 24;
            fprintf(stderr, "loool\n");
        }
            
        
        Graphics_render();   
        Platform_refreshWindow();
        Platform_sleep(0.01);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");
    
    
    LinkedList_deleteContents(objects, Drawable_delete);
    LinkedList_delete(objects);

    Graphics_terminate();
    
    Platform_closeWindow();   
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();

    return 0;
}
