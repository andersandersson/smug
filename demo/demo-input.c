#include "stdlib.h"
#include "stdio.h"
#include <math.h>

#include "platform/platform.h"
#include "input/input.h"

//#include "platform/opengl/opengl.h"

#include "common/log.h"
#include "common/common.h"

#include "graphics/graphics.h"
#include "graphics/renderer/renderer.h"
#include "graphics/drawable/drawable.h"
#include "graphics/sprite.h"
#include "graphics/drawable/box.h"

#include "graphics/texture/texture.h"
#include "graphics/image/image.h"
#include "graphics/camera.h"


#define SWIDTH 640
#define SHEIGHT 480

float myRandom(float f)
{
    return f * ((float)rand() / (float)RAND_MAX);
}

int main()
{
    if (!Log_init())
		return 0;

    if (!Platform_init())
        return 0;
    
    Log_print("Initializing\n");

	
    Platform_openWindow(640, 480, FALSE);    

	if (!Input_init())
		return 0;


    if (!Graphics_init(640, 480))
        return 0;


	Drawable* paddle1 = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -32, 16, 64)); 
	Drawable* paddle2 = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -32, 16, 64)); 
	Drawable* ball = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -8, 16, 16));

	Drawable_setPos(paddle1, Point_createFromXY(-300, 0));
	Drawable_setPos(paddle2, Point_createFromXY(300, 0));
	Drawable_setPos(ball, Point_createFromXY(0, 0));
	Drawable_setColor(paddle1, Color_createFromRGBA(0,0,1,1));
	Drawable_setColor(paddle2, Color_createFromRGBA(0,1,0,1));
	Drawable_setColor(ball, Color_createFromRGBA(1,0,0,1));

	Graphics_addDrawable(paddle1);
	Graphics_addDrawable(paddle2);
	Graphics_addDrawable(ball);
	
    Log_print("Running\n");
    TIME t = Platform_getTime();
    TIME nexttime = t;
    TIME lastFpsCheck = t;
    TIME delay = 1.0f/60.0f;
    int fps = 0;
    while (1)
    {   
		Platform_update();
        t = Platform_getTime();
             
        if (t >= nexttime)
        {
            nexttime+=delay;

            if (Input_getKey(KEY_ESC))
                break;
                
            // Stuff of interest    
            {    
				
		
		
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
        
        Platform_sleep(nexttime - t);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");

	Drawable_delete(paddle1);
	Drawable_delete(paddle2);
	Drawable_delete(ball);
    
    Graphics_terminate();
    
    Platform_closeWindow();   
    
	Input_terminate();
	
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();

    return 0;
}
