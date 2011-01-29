#include <math.h>
#include <stdlib.h>

#include <utils/shapes.h>
#include <physics/physics.h>
#include <physics/body.h>
#include <platform/platform.h>
#include <graphics/graphics.h>
#include <common/log.h>
#include <common/common.h>
#include <utils/rectangle.h>

#include "input/input.h"

int handle_collision = 0;

int collision_hook(void* lparam, void* rparam)
{
  return 0;
}

int main(void)
{
    if (!Log_init())
        return 0;

    if (!Platform_init(640, 480, FALSE))
        return 0;

    if (!Input_init())
        return 0;

    if (!Physics_init())
        return 0;

    if (!Graphics_init())
        return 0;

    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;

    Physics_setDefaultCollisionHandler(1, 0);
    Physics_setDefaultCollisionHandler(1, 1);

    Body* left_wall = Body_new();
    Body* right_wall = Body_new();
    Body* top_wall = Body_new();
    Body* bottom_wall = Body_new();

    left_wall->type = 0;
    right_wall->type = 0;
    top_wall->type = 0;
    bottom_wall->type = 0;

    left_wall->immovable = TRUE;
    right_wall->immovable = TRUE;
    top_wall->immovable = TRUE;
    bottom_wall->immovable = TRUE;

    Body_setShape(left_wall, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 10.0, 480.0)));
    Body_setShape(right_wall, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 10.0, 480.0)));
    Body_setShape(top_wall, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 640.0, 10.0)));
    Body_setShape(bottom_wall, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 640.0, 10.0)));

    Body_setPosition(left_wall, Point_createFromXY(-320.0, -240.0));
    Body_setPosition(right_wall, Point_createFromXY(310.0, -240.0));
    Body_setPosition(top_wall, Point_createFromXY(-320.0, -240.0));
    Body_setPosition(bottom_wall, Point_createFromXY(-320.0, 230.0));

    Hook* hook = Hook_newFromFunction(NULL, collision_hook);
    //Physics_addCollisionHook(1, 0, hook);
    //Physics_addCollisionHook(1, 1, hook);

    Physics_addBody(left_wall);
    Physics_addBody(right_wall);
    Physics_addBody(top_wall);
    Physics_addBody(bottom_wall);

    int i;
    for(i = 0; i < 8; i++)
    {
        int x = 8+i%2; //i % 10;
        int y = i / 4;

        Body* ball = Body_new();
        Body_setShape(ball, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 30.0, 30.0)));
        Body_setPosition(ball, Point_createFromXY(x*45.0 - 300.0, y*45.0 - 220));
        Body_setVelocity(ball, Vector_create2d(50.0, 0.0));
        //Body_setPosition(ball, Point_createFromXY(x*30.0, y*30.0-170));
        //Body_setVelocity(ball, Vector_create2d(100.0, 0.0));
        ball->mass = 1.0;
        ball->elasticity = 0.5;
	ball->type = 1;
	Physics_addBody(ball);
    }

    Body* ball1 = Body_new();
    Body_setShape(ball1, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 30.0, 30.0)));
    Body_setPosition(ball1, Point_createFromXY(0.0, 0.0));
    Body_setVelocity(ball1, Vector_create2d(10.0, 10.0));
    ball1->mass = 1.0;
    ball1->elasticity = 1.0;
    ball1->type = 1;
    //Physics_addBody(ball1);

    Body* ball2 = Body_new();
    Body_setShape(ball2, Shape_newFromRectangle(Rectangle_createFromXYWH(0.0, 0.0, 30.0, 30.0)));
    Body_setPosition(ball2, Point_createFromXY(30.0,30.0));
    Body_setVelocity(ball2, Vector_create2d(-10.0, -10.0));
    ball2->mass = 1.0;
    ball2->elasticity = 1.0;
    ball2->type = 1;
    //Physics_addBody(ball2);

    int space_lock = 1;
    BOOL do_update = FALSE;
    //Body_setVelocity(ball1, Vector_create2d(100.0, 0.0));
    while (1)
        {
            Physics_update(do_update||TRUE);
            do_update = FALSE;

            if (Input_getKey(KEY_ESC))
                break;

            t = Platform_getTime();

	    if(0 == space_lock && Input_getKey(KEY_SPACE))
	      {
		do_update = TRUE;
		space_lock = 1;
	      }

	    if(1 == space_lock && !Input_getKey(KEY_SPACE))
	      {
		space_lock = 0;
	      }

	    if(Input_getKey(KEY_UP))
	      {
                ball1->velocity = Vector_add(ball1->velocity, Vector_create2d(0.0, -100.0));
	      }

	    if(Input_getKey(KEY_DOWN))
            {
                ball1->velocity = Vector_add(ball1->velocity, Vector_create2d(0.0, 100.0));
            }

	    if(Input_getKey(KEY_LEFT))
            {
                ball1->velocity = Vector_add(ball1->velocity, Vector_create2d(-100.0, 0.0));
            }

	    if(Input_getKey(KEY_RIGHT))
            {
                ball1->velocity = Vector_add(ball1->velocity, Vector_create2d(100.0, 0.0));
            }
            //Body_setVelocity(ball1, Vector_create2d(100.0, 0.0));

	    if(Input_getKey(KEY_SPACE))
            {
                //ball1->velocity = Vector_add(ball1->velocity, Vector_create2d(0.0, -100.0));
            }

            if(t >= next_t)
            {
                //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Graphics_render();
                Physics_checkCollisions(next_t*5, TRUE == do_update || Input_getKey(KEY_ENTER));
                next_t += 0.03;
                Platform_refreshWindow();
            }
	    Platform_sleep(next_t-t);
        }

    Hook_delete(hook);

    Graphics_terminate();

    Physics_terminate();

    Platform_terminate();

    Log_terminate();

    return 0;
}
