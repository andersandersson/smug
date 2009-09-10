
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
    TIME t = Platform_getTime();
    float dir = 1;
    float deltadir = 0.002f;
    float color = 0;
    TIME nexttime = t;
    TIME lastFpsCheck = t;
    TIME delay = 1.0f/60.0f;
    int fps = 0;
    while (1)
    {   
        t = Platform_getTime();
             
        if (t >= nexttime)
        {
            nexttime+=delay;

            if (Platform_getKey(KEY_ESC))
                break;
                
            // Stuff of interest    
            {    
                target = Point_createFromXY((SWIDTH) * cosf(t*2) + SWIDTH / 2, (SHEIGHT) * sinf(t*2) + SHEIGHT / 2);
            
                Node* p = objects->first;
                while (p != NULL)
                {
                    ((Drawable*)p->item)->pos = Point_addVector(((Drawable*)p->item)->pos, 
                                                                    Vector_multiply(Point_distanceToPoint(((Drawable*)p->item)->pos, target), dir * (0.003 + myRandom(0.035))));
                    ((Box*)p->item)->color = Color_createFromRGBA(0.5+sinf((color*3+0)*1.3)*0.5, 0.5+sinf((color*3+1)*1.3)*0.5, 0.5+sinf((color*3+2)*1.3)*0.5, 0.33);         
                    p = p->next;
                }

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
                
                color+= 0.0015;
                if (color > 24)
                {
                    color -= 24;
                    fprintf(stderr, "loool\n");
                }
            }

            Graphics_render();
            Platform_refreshWindow();
            
            fps++;
        }

        if (t - lastFpsCheck >= 1.0)
        {
            //Console_writeLine("Fps: %i", fps);
            //Log_write(LOG_NOTIFICATION, "FPSCK", "engine.c", 116, "%i", fps);
            Log_printLine("Fps: %i", fps);
            fps = 0;
            lastFpsCheck = t;
        }
        
        // TODO: add if-case: if nexttime-time > smallest_sleep_time
        Platform_sleep(nexttime - t);
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
