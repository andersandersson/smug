#include "physics/shapes.h"
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

int main()
{
    Log_init();
    
    if (!Platform_init(640, 480, FALSE))
        return 0;
        
    Log_print("Initializing\n");
    
    if (!Input_init())
        return 0;
    
    if (!Physics_init())
        return 0;       

    if (!Graphics_init(640, 480))
        return 0;


    float t1, t2, t3, t4; 
    
    _collideMovingInterval1D(434.0, 10.0, 30.0, 30.0,
			     0.0, 0.0, 30.0, 10.0, &t1, &t2, &t3, &t4);
    DEBUG("%f, %f", t1, t2);
    

    _collideInterval1D(5.0, 2.0, 6.0, 2.0, &t1);
    DEBUG("%f", t1);
    _collideInterval1D(6.0, 2.0, 5.0, 2.0, &t1);
    DEBUG("%f", t1);
    //    exit(0);
    Shape* shape1 = Shape_createFromRectangle(Rectangle_createFromXYWH(-15.0, -15.0, 30.0, 30.0));
    Body* body1 = Body_new();
    body1->type = 1;
    body1->mass = 1.0;
    body1->elasticity = 1.0;
    Body_setPosition(body1, Point_createFromXY(195.755646, 426.516083));
    Body_setPosition(body1, Point_createFromXY(200.0, 100.0));
    Body_setShape(body1, shape1);

    Shape* shape2 = Shape_createFromRectangle(Rectangle_createFromXYWH(-15.0, -15.0, 30.0, 30.0));
    Body* body2 = Body_new();
    body2->type = 2;
    body2->mass = 1.0;
    body2->elasticity = 1.0;
    body2->immovable = TRUE;
    Body_setPosition(body2, Point_createFromXY(265.755646, 434.244415));
    Body_setPosition(body2, Point_createFromXY(200.0, 200.0));
    Body_setShape(body2, shape2);


    Shape* shape3 = Shape_createFromRectangle(Rectangle_createFromXYWH(-30.0, -30.0, 60.0, 60.0));
    Body* body3 = Body_new();
    body3->type = 1;
    Body_setPosition(body3, Point_createFromXY(100.0, 220.0));
    Body_setShape(body3, shape3);

    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;
    
    Physics_addBody(body1);
    Physics_addBody(body2);
    //    Physics_addBody(body3);

    Physics_setDefaultCollisionHandler(1,2);

    current_body = body1;
    int tab_lock = 0;
    int space_lock = 0;

    Body_setVelocity(body1, Vector_create2d(-14.999999, 15.0000067));
    Body_setVelocity(body2, Vector_create2d(14.9999857, 14.9999962));

    Body_setVelocity(body1, Vector_create2d(-15.0, 15.0));
    Body_setVelocity(body2, Vector_create2d(30.0, -30.0));

    Body_setVelocity(body1, Vector_create2d(0.0, 0.0));
    Body_setVelocity(body2, Vector_create2d(0.0, 0.0));
    BOOL do_update = FALSE;

    while (1)
        {   
            float x, y;

            if (Input_getKey(KEY_ESC))
                break;


            if (Input_getKey(KEY_DOWN))
                {
                    y = 10.0;
                } 
            else if(Input_getKey(KEY_UP))
                {
                    y = -10.0;
                }
            else
                {
                    y = 0.0;
                }


            if (Input_getKey(KEY_RIGHT))
                {
                    x = 10.0;
                } 
            else if(Input_getKey(KEY_LEFT))
                {
                    x = -10.0;
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
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    Physics_update(t,do_update);
		    do_update = FALSE;
                    next_t += 0.02;
		    Platform_refreshWindow();
                }
	    Platform_sleep(next_t-t);
        }

    Physics_removeBody(body1);
    Physics_removeBody(body2);
    Physics_removeBody(body3);

    Body_delete(body1);
    Shape_delete(shape1);
    Body_delete(body2);
    Shape_delete(shape2);
    Body_delete(body3);
    Shape_delete(shape3);

    //Hook_delete(hook);

    Log_print("Stopped\n");

    Graphics_terminate();

    Physics_terminate();
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();
 
    return 0;
}
