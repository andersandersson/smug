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
    CollisionData* data = (CollisionData*) rparam;

    Point position = data->self->position;
    Vector movement = Vector_multiply(data->movement, data->collisionTime);    
    Vector m = Vector_sub(data->movement, movement);
    Vector p = Vector_projection(m, data->normal);

    float f = Vector_dotProduct(data->normal, data->movement);
    position = Point_addVector(position, movement);
    position = Point_addVector(position, p);
    
    //Physics_drawShape(data->left->shape, position, Color_createFromRGBA(1.0,0.0,1.0,1.0));
    
    if(0 != handle_collision) 
        {	  
	  if(f <= 0.0f) {
	    data->self->new_position = position;
	  }
        }
}

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


    Shape* shape = Shape_createFromRectangle(Rectangle_createFromXYWH(-30.0, -30.0, 60.0, 60.0));
    Body* body = Body_new();
    Body_setPosition(body, 370.0, 300.0);
    Body_setShape(body, shape);

    Shape* shape2 = Shape_createFromRectangle(Rectangle_createFromXYWH(-40.0, -40.0, 80.0, 80.0));
    Body* body2 = Body_new();
    Body_setPosition(body2, 300.0, 100.0);
    Body_setShape(body2, shape2);

    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;

    Physics_addBody(body);
    Physics_addBody(body2);

    Hook* hook = Hook_newFromFunction(NULL, collision_hook);
    
    Physics_addCollisionHook(body->type, body2->type, hook);
    Physics_addCollisionHook(body->type+1, body2->type, hook);

    Body* other_body = body;
    Body* current_body = body2;
    int tab_lock = 0;
    int space_lock = 0;
    float x_dir = 5.0;
    float y_dir = 2.0;
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

            if (Input_getKey(KEY_ENTER))
	    {
	      current_body->position = current_body->new_position;
	    }

            if (Input_getKey(KEY_TAB))
                {
                    if(tab_lock == 0)
                        {
                            tab_lock = 1;
                            if(current_body == body2) 
                                {
                                    current_body = body;
                                }
                            else
                                {
                                    current_body = body2;
                                }
                        }
                } 
            else
                {
                    tab_lock = 0;
                }

            if (Input_getKey(KEY_SPACE))
                {
                    if(space_lock == 0)
                        {
                            space_lock = 1;
                            handle_collision = 1 - handle_collision;
                        }
                } 
            else
                {
                    space_lock = 0;
                }

            if(Point_getX(&other_body->position) >= 640 ||
               Point_getX(&other_body->position) <= 0)
                {
                    x_dir *= -1;
                }

            if(Point_getY(&other_body->position) >= 480 ||
               Point_getY(&other_body->position) <= 0)
                {
                    y_dir *= -1;
                }


            Body_move(other_body, x_dir, y_dir);
            Body_move(current_body, x, y);

            t = Platform_getTime();

            if(t >= next_t)
                {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    Physics_update(0);
                    next_t += 0.03;
		    Platform_refreshWindow();
                }
	    Platform_sleep(next_t-t);
        }
    Physics_removeBody(body);
    Physics_removeBody(body2);

    Body_delete(body);
    Shape_delete(shape);
    Body_delete(body2);
    Shape_delete(shape2);

    Hook_delete(hook);
    
    Log_print("Stopped\n");

    Graphics_terminate();

    Physics_terminate();
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();
 
    return 0;
}
