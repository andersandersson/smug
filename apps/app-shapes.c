#include "physics/shapes.h"
#include "physics/physics.h"
#include "physics/body.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "common/log.h"
#include "common/common.h"

#include "input/input.h"

int collision_hook(void* lparam, void* rparam)
{
    CollisionData* data = (CollisionData*) rparam;

    DEBUG("Result (%f): %f, %f", Rectangle_getW((Rectangle*)data->left->shape->data), Point_getX(data->result), Point_getY(data->result));
    data->left->position = Point_addVector(data->left->position, *data->result);
    data->left->new_position = data->left->position;
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


    Shape* shape = Shape_createFromRectangle(Rectangle_createFromXYWH(10.0, 10.0, 60.0, 60.0));
    Body* body = Body_new();
    Body_setPosition(body, 300.0, 300.0);
    Body_setShape(body, shape);

    Shape* shape2 = Shape_createFromRectangle(Rectangle_createFromXYWH(10.0, 10.0, 80.0, 80.0));
    Body* body2 = Body_new();
    Body_setPosition(body2, 100.0, 100.0);
    Body_setShape(body2, shape2);

    TIME t, next_t;
    t = Platform_getTime();
    next_t = t;

    Physics_addBody(body);
    Physics_addBody(body2);

    Hook* hook = Hook_newFromFunction(NULL, collision_hook);
    
    Physics_addCollisionHook(body->type, body2->type, hook);
    Physics_addCollisionHook(body->type+1, body2->type, hook);

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

    Hook_delete(hook);
    
    Log_print("Stopped\n");

    Graphics_terminate();

    Physics_terminate();
    
    Platform_terminate();
    
    Log_print("Done\n");
    
    Log_terminate();
 
    return 0;
}
