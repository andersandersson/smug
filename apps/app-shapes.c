#include "utils/shapes.h"
#include "utils/rectangle.h"
#include "physics/physics.h"
#include "physics/body.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "common/log.h"
#include "common/common.h"

#include "input/input.h"

int handle_collision = 0;

int collision_hook(void* lparam, void* rparam)
{
}

extern Body* current_body;

int main(void)
{
    Log_init();

    if (!Platform_init(640, 480, FALSE))
        return 0;

    Log_print("Initializing\n");

    if (!Input_init())
        return 0;

    if (!Physics_init())
        return 0;

    if (!Graphics_init())
        return 0;


    float t1, t2, t3, t4;

    //    exit(0);
    Shape* shape1 = Shape_newFromRectangle(Rectangle_createFromXYWH(-15.0, -15.0, 30.0, 30.0));
    Body* body1 = Body_new();
    body1->type = 1;
    body1->mass = 1.0;
    body1->elasticity = 1.0;
    Body_setPosition(body1, Point_createFromXY(200.0, 195.0));
    Body_setShape(body1, shape1);

    Shape* shape2 = Shape_newFromRectangle(Rectangle_createFromXYWH(-15.0, -15.0, 30.0, 30.0));
    Body* body2 = Body_new();
    body2->type = 1;
    body2->mass = 2.0;
    body2->elasticity = 1.0;
    body2->immovable = FALSE;
    Body_setPosition(body2, Point_createFromXY(200.0, 200.0));
    Body_setShape(body2, shape2);


    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;

    Physics_addBody(body1);
    Physics_addBody(body2);
    //    Physics_addBody(body3);

    Physics_setDefaultCollisionHandler(1,1);

    current_body = body1;
    int tab_lock = 0;
    int space_lock = 0;

    Body_setVelocity(body1, Vector_create2d(-14.999999, 15.0000067));
    Body_setVelocity(body2, Vector_create2d(14.9999857, 14.9999962));

    Body_setVelocity(body1, Vector_create2d(0.0, 30.0));
    Body_setVelocity(body2, Vector_create2d(0.0, 0.0));

    //Body_setVelocity(body1, Vector_create2d(0.0, 0.0));
    //Body_setVelocity(body2, Vector_create2d(0.0, 0.0));
    BOOL do_update = FALSE;

    while (1)
        {
            float x, y;

            if (Input_getKey(KEY_ESC))
                break;


            if (Input_getKey(KEY_DOWN))
                {
                    y = 30.0;
                }
            else if(Input_getKey(KEY_UP))
                {
                    y = -30.0;
                }
            else
                {
                    y = 0.0;
                }


            if (Input_getKey(KEY_RIGHT))
                {
                    x = 30.0;
                }
            else if(Input_getKey(KEY_LEFT))
                {
                    x = -30.0;
                }
            else
                {
                    x = 0.0;
                }

            if (Input_getKey(KEY_TAB))
                {
                    if(tab_lock == 0)
                        {
                            tab_lock = 1;
                            if(current_body == body1)
			      {
				current_body = body2;
			      }
                            else if(current_body == body2)
			      {
				current_body = body1;
			      }
			    else
			      {
				current_body = body1;
			      }
                        }
                }
            else
                {
                    tab_lock = 0;
                }

            if (Input_getKey(KEY_ENTER))
                {
                    if(space_lock == 0)
                        {
                            space_lock = 1;
			    do_update = TRUE;
                        }
                }
            else
                {
                    space_lock = 0;
                }
            t = Platform_getTime();

	    Vector vel = current_body->velocity;
	    Vector vel_add = Vector_create2d(x, y);
	    vel = Vector_add(vel, vel_add);
	    Body_setVelocity(current_body, vel);

            if(t >= next_t)
                {
                    Graphics_render();
                    Physics_update(next_t,TRUE);
		    do_update = FALSE;
                    next_t += 0.02;
		    Platform_refreshWindow();
                }
	    Platform_sleep(next_t-t);
        }

    Physics_removeBody(body1);
    Physics_removeBody(body2);

    Body_delete(body1);
    Shape_delete(shape1);
    Body_delete(body2);
    Shape_delete(shape2);

    //Hook_delete(hook);

    Log_print("Stopped\n");

    Graphics_terminate();

    Physics_terminate();

    Platform_terminate();

    Log_print("Done\n");

    Log_terminate();

    return 0;
}
