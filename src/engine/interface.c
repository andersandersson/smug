#include <common/common.h>
#include <common/log.h>
#include "engine.h"
#include <utils/rectangle.h>
#include <graphics/color.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/box.h>
#include <engine/gameobject.h>
#include <platform/platform.h>
#include <smugstd.h>
#include "interface.h"

SMUGEXPORT int smugInit(void)
{
    return Engine_init(TRUE, TRUE);
}

SMUGEXPORT void smugTerminate(void)
{
    Engine_terminate();
}

SMUGEXPORT int smugAddObject(SmugObject obj)
{
    Engine_addObject(obj);
    return 0;
}

SMUGEXPORT SmugObject smugObject_new(void)
{
    return GameObject_new();
}

SMUGEXPORT void smugObject_delete(SmugObject object)
{
    GameObject_delete(object);
}

SMUGEXPORT void smugObject_setPos(SmugObject obj, float x, float y)
{
    GameObject_setPos(obj, x, y);
}

SMUGEXPORT void smugObject_addDrawable(SmugObject obj, SmugDrawable d)
{
    GameObject_addDrawable(obj, d);
}

/* Commenting this out for now. Want to have possibility of compiling with or without a gameloop internal to smug. */
/* void smugRun(void)
{
    if (!Engine_isInitialized())
    {
        ERROR("Engine must be initialized before calling smugRun()");
    }

    Engine_run();
} */
