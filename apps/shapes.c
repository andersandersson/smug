#include "physics/shapes.h"
#include "physics/physics.h"
#include "physics/body.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "common/log.h"
#include "common/common.h"


int main(char argc, char* argv[])
{
    Log_init();
    
    if (!Platform_init())
        return 0;
    
    Log_print("Initializing\n");
    Physics_init();

    Platform_openWindow(640, 480, FALSE);    

    if (!Graphics_init(640, 480))
        return 0;


    Shape* shape = Shape_createFromRectangle(Rectangle_createFromXYWH(10.0, 10.0, 300.0, 80.0));
    Body* body = Body_new();
    Body_setShape(body, shape);

    Shape* shape2 = Shape_createFromRectangle(Rectangle_createFromXYWH(10.0, 10.0, 300.0, 80.0));
    Body* body2 = Body_new();
    body2->position = Point_createFromXY(100.0, 100.0);
    Body_setShape(body2, shape2);

    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;

    Physics_addBody(body);
    Physics_addBody(body2);
    while (1)
        {   
            float x, y;

            if (Platform_getKey(KEY_ESC))
                break;


            if (Platform_getKey(KEY_DOWN))
                {
                    y = 10.0;
                } 
            else if(Platform_getKey(KEY_UP))
                {
                    y = -10.0;
                }
            else
                {
                    y = 0.0;
                }


            if (Platform_getKey(KEY_RIGHT))
                {
                    x = 10.0;
                } 
            else if(Platform_getKey(KEY_LEFT))
                {
                    x = -10.0;
                }
            else
                {
                    x = 0.0;
                }

            Body_move(body2, x, y);

            t = Platform_getTime();

            if(t >= next_t)
                {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    Physics_update(0);
                    next_t += 0.03;
                }

            Platform_refreshWindow();
        }
    Physics_removeBody(body);
    Physics_removeBody(body2);

    Body_delete(body);
    Shape_delete(shape);
    Body_delete(body2);
    Shape_delete(shape2);
    
    Log_print("Stopped\n");

    Graphics_terminate();

    Physics_terminate();
    
    Platform_closeWindow();   
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();
 
    return 0;
}
