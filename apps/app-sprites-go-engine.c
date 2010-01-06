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

#include "engine/engine.h"

#define SWIDTH 640
#define SHEIGHT 480

float myRandom(float f)
{
    return f * ((float)rand() / (float)RAND_MAX);
}

static Camera* camera = NULL;
static float rot = 0.0f;
static float zoom = 1.0f;
static Vector cpos;
static float color = 0;
static LinkedList* drawables;
static LinkedList* gameObjects;

void myLogicCallback()
{
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
        Node* p = gameObjects->first;
        Node* q = drawables->first;
        Drawable* d;
        while (p != NULL)
        {
            assert(q != NULL);
            d = (Drawable*)q->item;
            Drawable_setColor(d, Color_createFromRGBA(  0.5 + sinf((color*3 + 0)*1.3)*0.5,
                                                        0.5 + sinf((color*3 + 1)*1.3)*0.5,
                                                        0.5 + sinf((color*3 + 2)*1.3)*0.5,
                                                        0.50));
            p = p->next;
            q = q->next;
        }

        color += 0.0015;
        if (color > 24)
        {
            color -= 24;
        }
    }
}

int main()
{
    /*******************
        Initialization
    *******************/

    if (!Engine_init(TRUE, FALSE))
        return 1;

    Log_setLevel(LOG_ALL);

    /*******************
        Setup
    *******************/

    camera = Graphics_getCamera();
    cpos = Vector_create2d(0,0);
    drawables = LinkedList_new();
    gameObjects = LinkedList_new();

    Image* image[4];
    Texture* texture[4];
    Sprite* sprite[4];

    image[0] = Image_new();
    Image_loadFromFile(image[0], "box.png");
    image[1] = Image_new();
    Image_loadFromFile(image[1], "box2.png");
    image[2] = Image_new();
    Image_loadFromFile(image[2], "box3.png");
    image[3] = Image_new();
    Image_loadFromFile(image[3], "box4.png");

    int j;
    for (j = 0; j < 4; j++)
    {
        texture[j] = Texture_newFromImage(image[j]);
        sprite[j] = Sprite_newFromPixelCoords(texture[j], Rectangle_createFromXYWH(0, 0, 64,64), Vector_create2d(32,32));
    }

    Graphics_setupLayer(0, 0.6f);
    Graphics_setupLayer(1, 0.8f);
    Graphics_setupLayer(2, 1.0f);
    Graphics_setupLayer(3, 1.2f);

    int i = 0;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < (4-j) * (4-j) * 60; i++)
        {
            float r = myRandom(100);
            float x = myRandom((j+1) * 5) + (j+1) * 4;

            Drawable* newDrawable = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-x, -x, x*2, x*2));
            Drawable_setSprite(newDrawable, sprite[j]);
            Drawable_setLayer(newDrawable, j);

            GameObject* newGO;
            newGO = GameObject_new();
            GameObject_addDrawable(newGO, newDrawable);
            GameObject_setPos(newGO, (SWIDTH/2 * myRandom(3.2)) * cosf(r), (SHEIGHT/2 * myRandom(3.2)) * sinf(r));

            LinkedList_addLast(drawables, newDrawable);
            LinkedList_addLast(gameObjects, newGO);

            Engine_addObject(newGO);
        }
    }

    /*******************
        "Game" loop
    *******************/

    Engine_setLogicCallback(myLogicCallback);
    Engine_enableLogicCallback(TRUE);

    Engine_run();

    /*******************
        Cleanup
    *******************/

    NOTIFY("Stopped.");

    NOTIFY("Cleaning up.");

    LinkedList_deleteContents(drawables, Drawable_delete);
    LinkedList_deleteContents(gameObjects, GameObject_delete);
    LinkedList_delete(drawables);
    LinkedList_delete(gameObjects);

    for (j = 0; j < 4; j++)
    {
        Sprite_delete(sprite[j]);
        Texture_delete(texture[j]);
        Image_delete(image[j]);
    }

    Engine_terminate();

    return 0;
}
