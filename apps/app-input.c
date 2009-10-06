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

    if (!Platform_init(SWIDTH, SHEIGHT, FALSE))
        return 0;
    
	if (!Input_init())
		return 0;

    if (!Graphics_init(SWIDTH, SHEIGHT))
        return 0;
    
    Point boxpos = Point_createFromXY(0, 0);
    
	Drawable* box = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-8, -32, 16, 64)); 
	Drawable_setPos(box, boxpos);
	Drawable_setColor(box, Color_createFromRGBA(1,1,0,1));
	
	Graphics_addDrawable(box);
	
    // Define controller types for controlling paddle and gui
    ControllerType* masterControllerType = ControllerType_new(1, 2);
    // Create instances of controllers
    Controller* master = Controller_new(masterControllerType);
    // Bind master control to input and connect
    Controller_bindButton(master, 0, DEVICE_KEYBOARD, 'Z');
    Controller_bindAxis(master, 0, DEVICE_KEYBOARD, KEY_LEFT, DEVICE_KEYBOARD, KEY_RIGHT);
    Controller_bindAxis(master, 1, DEVICE_KEYBOARD, KEY_DOWN, DEVICE_KEYBOARD, KEY_UP);
    Input_connectController(master, 0);
 
    Log_print("Running\n");
    TIME t = Platform_getTime();
    TIME nexttime = t;
    TIME lastFpsCheck = t;
    TIME delay = 1.0f/20.0f;
    int fps = 0;
    Vector cpos = Vector_create2d(0,0);
    float zoom = 1;
    float rot = 0;
    Camera* camera = Graphics_getCamera();
    int col = 0;
    int debug = 0;
    while (1)
    {   
		Platform_update();
        t = Platform_getTime();
  
        if (t >= nexttime)
        {
            nexttime+=delay;
                
            if (Input_getKey(KEY_ESC))
                break;
                
            if (Input_getKey(KEY_F1))
                Graphics_setRenderMode(RENDER_NORMAL);
            if (Input_getKey(KEY_F2))
                Graphics_setRenderMode(RENDER_ALL);
                
            // Camera controls    
            Vector mchange = Input_getMouseScreenMovement();
            if (Vector_squareLength(mchange))
            {
                if (Input_getMouseButton(MOUSE_BUTTON_LEFT))
                {
                    cpos = Vector_add(cpos, mchange);
                }
                else if (Input_getMouseButton(MOUSE_BUTTON_RIGHT))
                {
                    zoom += 0.05 * (Vector_getX(&mchange) + Vector_getY(&mchange));
                }
                else if (Input_getMouseButton(MOUSE_BUTTON_MIDDLE))
                {
                    rot += Vector_getX(&mchange) + Vector_getY(&mchange);
                }
            
                Camera_setPosition(camera, Point_createFromVector(cpos));
                Camera_setZoom(camera, zoom);
                Camera_setRotation(camera, rot);
            }

    
            // Stuff of interest    
            {    
                    
                boxpos = Point_addVector(boxpos, Vector_create2d(Controller_getAxisValue(master, 0) * 8, 
                                                                    Controller_getAxisValue(master, 1) * -8));   
                
                if (Controller_wasButtonTriggered(master, 0))
                {
                    if (col)
                        Drawable_setColor(box, Color_createFromRGBA(col--, 0, 0, 1));
                    else
                        Drawable_setColor(box, Color_createFromRGBA(1-col++, 1-col, 0, 1));
                }
                                
                Drawable_setPos(box, boxpos);
            }

            Graphics_render();
            
            Platform_refreshWindow();

            fps++;
        }

        if (t - lastFpsCheck >= 1.0)
        {
            Log_printLine("Fps: %i", fps);
            fps = 0;
            lastFpsCheck = t;
        }
        
        Platform_sleep(nexttime - t);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");

    Drawable_delete(box);
    
    Graphics_terminate();
    
	Input_terminate();	
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();

    return 0;
}
