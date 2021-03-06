
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "platform/platform.h"

//#include "platform/opengl/opengl.h"

#include "common/log.h"
#include "common/common.h"

#include "input/input.h"

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
    Log_init();

    if (!Platform_init(640, 480, FALSE))
        return 0;

    Log_print("Initializing\n");

	if (!Input_init())
        return 0;

    if (!Graphics_init(640, 480))
        return 0;

    Image* image[4];

    image[0] = Image_new();
    Image_loadFromFile(image[0], "box.png");
    image[1] = Image_new();
    Image_loadFromFile(image[1], "box2.png");
    image[2] = Image_new();
    Image_loadFromFile(image[2], "box3.png");
    image[3] = Image_new();
    Image_loadFromFile(image[3], "box4.png");

    Texture* texture[4];
    Sprite* sprite[4];

    int j;
    for (j = 0; j < 4; j++)
    {
        texture[j] = Texture_newFromImage(image[j]);
    }
    for (j = 0; j < 4; j++)
    {
        sprite[j] = Sprite_newFromPixelCoords(texture[j], Rectangle_createFromXYWH(0, 0, 64,64), Vector_create2d(32,32));
    }

    Graphics_setupLayer(0, 0.6f);
    Graphics_setupLayer(1, 0.8f);
    Graphics_setupLayer(2, 1.0f);
    Graphics_setupLayer(3, 1.2f);

    LinkedList* objects = LinkedList_new();

    int i = 0;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < (4-j) * (4-j) * 60; i++)
        {

            float r = myRandom(100);
            float x = myRandom((j+1) * 5) + (j+1) * 4;

            Drawable* drawable = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-x, -x, x*2, x*2));
            Drawable_setPos(drawable, Point_createFromXY((SWIDTH/2 * myRandom(3.2)) * cosf(r), (SHEIGHT/2 * myRandom(3.2)) * sinf(r)));
            Drawable_setSprite(drawable, sprite[j]);
            Drawable_setLayer(drawable, j);
            Graphics_addDrawable((Drawable*)drawable);
            LinkedList_addLast(objects, drawable);
        }
    }

    Log_print("Running\n");
    TIME t = Platform_getTime();
	float color = 0;
    TIME nexttime = t;
    TIME lastFpsCheck = t;
    TIME delay = 1.0f/60.0f;
    int fps = 0;
    Camera* camera = Graphics_getCamera();
    float rot = 0.0f;
    float zoom = 1.0f;
    Vector cpos = Vector_create2d(0,0);
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
				Node* p = objects->first;
                while (p != NULL)
                {
                    Drawable_setColor((Drawable*)p->item, Color_createFromRGBA(0.5+sinf((color*3+0)*1.3)*0.5, 0.5+sinf((color*3+1)*1.3)*0.5, 0.5+sinf((color*3+2)*1.3)*0.5, 0.50));
                    p = p->next;
                }

				color+= 0.0015;
                if (color > 24)
                {
                    color -= 24;
                }
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

        // TODO: add if-case: if nexttime-time > smallest_sleep_time
        Platform_sleep(nexttime - t);
    }

    Log_print("Stopped\n");

    Log_print("Cleaning up\n");


    LinkedList_deleteContents(objects, Drawable_delete);
    LinkedList_delete(objects);

    for (j = 0; j < 4; j++)
    {
        Sprite_delete(sprite[j]);
        Texture_delete(texture[j]);
        Image_delete(image[j]);
    }

    Graphics_terminate();

	Input_terminate();

    Platform_terminate();

    Log_print("Done\n");

    Log_terminate();

    return 0;
}
