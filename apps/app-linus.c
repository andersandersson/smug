/*

Try hitting keys 1, Q, 2, W, and so on...

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

typedef struct Guy
{
    GameObject* go;
    Drawable* parts[7];
} Guy;

float myRandom(float f)
{
    return f * ((float)rand() / (float)RAND_MAX);
}

static Camera* camera = NULL;
static Vector cpos;
static float rot = 0.0f;
static float zoom = 1.0f;

static float period1 = 0;
static float period2 = 0;
static Guy* guys[5];
static const float partSize = 10;

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

    if (Input_getKey('1'))
        GameObject_setOpacity(guys[0]->go, 0.0f);
    if (Input_getKey('Q'))
        GameObject_setOpacity(guys[0]->go, 1.0f);
    if (Input_getKey('2'))
        GameObject_setOpacity(guys[1]->go, 0.0f);
    if (Input_getKey('W'))
        GameObject_setOpacity(guys[1]->go, 1.0f);
    if (Input_getKey('3'))
        GameObject_setOpacity(guys[2]->go, 0.0f);
    if (Input_getKey('E'))
        GameObject_setOpacity(guys[2]->go, 1.0f);
    if (Input_getKey('4'))
        GameObject_setOpacity(guys[3]->go, 0.0f);
    if (Input_getKey('R'))
        GameObject_setOpacity(guys[3]->go, 1.0f);
    if (Input_getKey('5'))
        GameObject_setOpacity(guys[4]->go, 0.0f);
    if (Input_getKey('T'))
        GameObject_setOpacity(guys[4]->go, 1.0f);

    // Stuff of interest
    GameObject_setPos(guys[4]->go, 0, sinf(period1)*100);

    int i;
    for (i = 0; i < 5; i++)
    {
        Drawable_setPosRelative(guys[i]->parts[3], Point_createFromXY(-5*partSize - cosf(period2*2)*partSize/2, partSize + sinf(period2)*partSize));
        Drawable_setPosRelative(guys[i]->parts[4], Point_createFromXY(3*partSize + cosf(period2*2)*partSize/2, partSize + sinf(period2)*partSize));
        Drawable_setPosRelative(guys[i]->parts[5], Point_createFromXY(-4*partSize + sinf(period2)*partSize, 8*partSize + sinf(period2)*partSize));
        Drawable_setPosRelative(guys[i]->parts[6], Point_createFromXY(2*partSize - sinf(period2)*partSize, 8*partSize + sinf(period2)*partSize));
    }

    period1 += 0.03;
    period2 += 0.1;
}

Guy* Guy_new()
{
    return (Guy*)malloc(sizeof(Guy));
}

Guy* makeGuy(Color color)
{
    Image* image;
    Texture* texture;
    Sprite* sprite;
    Drawable* drawable;
    GameObject* object;
    Guy* guy;

    guy = Guy_new();
    image = Image_new();
    Image_loadFromFile(image, "box.png");
    texture = Texture_newFromImage(image);
    sprite = Sprite_newFromPixelCoords(texture, Rectangle_createFromXYWH(0, 0, 64,64), Vector_create2d(32,32));
    object = GameObject_new();

    int i;
    for (i = 0; i < 7; i++)
    {
        drawable = Drawable_newBoxFromRectangle(Rectangle_createFromXYWH(-partSize, -partSize, 2*partSize, 2*partSize));
        Drawable_setSprite(drawable, sprite);
        Drawable_setLayer(drawable, 0);
        Drawable_setColor(drawable, color);
        guy->parts[i] = drawable;
        GameObject_addDrawable(object, drawable);
    }
    guy->go = object;

    Drawable_setPosRelative(guy->parts[0], Point_createFromXY(-partSize, -partSize));
    Drawable_setPosRelative(guy->parts[1], Point_createFromXY(-partSize, 2*partSize));
    Drawable_setPosRelative(guy->parts[2], Point_createFromXY(-partSize, 5*partSize));
    Drawable_setPosRelative(guy->parts[3], Point_createFromXY(-4*partSize, partSize));
    Drawable_setPosRelative(guy->parts[4], Point_createFromXY(2*partSize, partSize));
    Drawable_setPosRelative(guy->parts[5], Point_createFromXY(-4*partSize, 8*partSize));
    Drawable_setPosRelative(guy->parts[6], Point_createFromXY(2*partSize, 8*partSize));

    return guy;
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

    Graphics_setupLayer(0, 1.0f);

    guys[0] = makeGuy(Color_createFromRGBA(1.0, 0.0, 0.0, 1.0));
    Engine_addObject(guys[0]->go);
    guys[1] = makeGuy(Color_createFromRGBA(0.0, 1.0, 0.0, 1.0));
    Engine_addObject(guys[1]->go);
    guys[2] = makeGuy(Color_createFromRGBA(0.0, 0.0, 1.0, 1.0));
    Engine_addObject(guys[2]->go);
    guys[3] = makeGuy(Color_createFromRGBA(1.0, 1.0, 0.0, 1.0));
    Engine_addObject(guys[3]->go);
    guys[4] = makeGuy(Color_createFromRGBA(0.0, 1.0, 1.0, 1.0));
    Engine_addObject(guys[4]->go);

    GameObject_setPos(guys[0]->go, -200, -100);
    GameObject_setPos(guys[1]->go, 200, -100);
    GameObject_setPos(guys[2]->go, 200, 100);
    GameObject_setPos(guys[3]->go, -200, 100);
    GameObject_setPos(guys[4]->go, 0, 0);

    /*******************
        "Game" loop
    *******************/

    Engine_setLogicCallback(myLogicCallback);
    Engine_enableLogicCallback(TRUE);

    Engine_run();

    /*******************
        Cleanup (not...)
    *******************/

    /* Bleeds memory like hell here. Who should be responsible
    for cleaning up images, textures and the like? */

    NOTIFY("Stopped.");

    Engine_terminate();

    return 0;
}
