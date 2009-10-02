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

    Log_print("Initializing\n");

    if (!Platform_init(640, 480, FALSE))
        return 0;
    
	if (!Input_init())
		return 0;

    if (!Graphics_init(640, 480))
        return 0;


    Graphics_setRenderMode(RENDER_ALL);

    float paddle1pos = 0;

	Drawable* paddle1 = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -32, 16, 64)); 
	Drawable* paddle2 = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -32, 16, 64)); 
	Drawable* ball = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -8, 16, 16));

	Drawable_setPos(paddle1, Point_createFromXY(-300, paddle1pos));
	Drawable_setPos(paddle2, Point_createFromXY(300, 0));
	Drawable_setPos(ball, Point_createFromXY(0, 0));
	Drawable_setColor(paddle1, Color_createFromRGBA(0,0,1,1));
	Drawable_setColor(paddle2, Color_createFromRGBA(0,1,0,1));
	Drawable_setColor(ball, Color_createFromRGBA(1,0,0,1));

	Graphics_addDrawable(paddle1);
	Graphics_addDrawable(paddle2);
	Graphics_addDrawable(ball);

    // Define controller types for controlling paddle and gui
    ControllerType* paddleControllerType = ControllerType_new(0, 1);
    ControllerType* masterControllerType = ControllerType_new(3, 2);
    // Create instances of controllers
    Controller* control1 = Controller_new(paddleControllerType);
    Controller* master = Controller_new(masterControllerType);
    // Bind master control to input and connect
    Controller_bindButton(master, 0, DEVICE_MOUSE, MOUSE_BUTTON_LEFT);
    Controller_bindButton(master, 1, DEVICE_MOUSE, MOUSE_BUTTON_RIGHT);
    Controller_bindButton(master, 2, DEVICE_MOUSE, MOUSE_BUTTON_MIDDLE);
    Controller_bindAxis(master, 0, DEVICE_MOUSE, MOUSE_AXIS_XNEG, DEVICE_MOUSE, MOUSE_AXIS_XPOS);
    Controller_bindAxis(master, 1, DEVICE_MOUSE, MOUSE_AXIS_YNEG, DEVICE_MOUSE, MOUSE_AXIS_YPOS);
    Input_connectController(control1, 0);
    // Bind paddle contol to input and connect
    Controller_bindAxis(control1, 0, DEVICE_KEYBOARD, KEY_DOWN, DEVICE_KEYBOARD, KEY_UP);
    Input_connectController(control1, 1);
    
 
    Log_print("Running\n");
    TIME t = Platform_getTime();
    TIME nexttime = t;
    TIME lastFpsCheck = t;
    TIME delay = 1.0f/20.0f;
    int fps = 0;
    float cx = 0;
    float cy = 0;
    float zoom = 1;
    float rot = 0;
    Camera* camera = Graphics_getCamera();
    while (1)
    {   
		Platform_update();
        t = Platform_getTime();
  
        if (t >= nexttime)
        {
            nexttime+=delay;
                
            if (Platform_getInputState(DEVICE_KEYBOARD, KEY_ESC))
                break;
                
            // Stuff of interest    
            {    
                    
                paddle1pos+= Controller_getAxisValue(control1, 0) * 8;
                Drawable_setPos(paddle1, Point_createFromXY(-300, paddle1pos));
                
                
                Point mchange = Point_createFromXY(Controller_getAxisChange(master, 0), Controller_getAxisChange(master, 1));
                if (Point_getX(&mchange) != 0.0f || Point_getY(&mchange) != 0.0f)
                {
                    fprintf(stderr, "lololo\n");
                    if (Controller_getButtonValue(master, 0))
                    {
                        cx += 1 * (Point_getX(&mchange));
                        cy += 1 * (Point_getY(&mchange));
                    }
                    else if (Controller_getButtonValue(master, 1))
                    {
                        zoom += 0.01 * (Point_getX(&mchange) + Point_getY(&mchange));
                    }
                    else if (Controller_getButtonValue(master, 2))
                    {
                        rot += 1 * (Point_getX(&mchange) + Point_getY(&mchange));
                    }
                
                    Camera_setPosition(camera, Point_createFromXY(-cx, -cy));
                    Camera_setZoom(camera, zoom);
                    Camera_setRotation(camera, rot);
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
        
        Platform_sleep(nexttime - t);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");

	Drawable_delete(paddle1);
	Drawable_delete(paddle2);
	Drawable_delete(ball);
    
    Graphics_terminate();
    
	Input_terminate();	
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();

    return 0;
}
