#include <smugstd.h>

#include <common/common.h>
#include <common/error.h>
#include <common/log.h>
#include <utils/rectangle.h>
#include <utils/point.h>
#include <graphics/color.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/box.h>
#include <platform/platform.h>
#include <engine/engine.h>
#include <engine/gameobject.h>
#include <engine/gameobjectiterator.h>
#include <engine/position_object.h>
#include <engine/interpoint.h>

#include <engine/interface.h>

#define DO_WRONG_TYPE_ERROR(obj, type, func) smug_error(GameObject_isType(obj, type), "Wrong type for " #func)

void smugRunTest(void)
{
    GameObject* go = GameObject_newGeneric();
    smug_assert(GameObject_isExactType(go, SMUG_TYPE_OBJECT));
    smug_assert(GameObject_isType(go, SMUG_TYPE_OBJECT));
    smug_assert(!GameObject_isExactType(go, SMUG_TYPE_BODY));
    smug_assert(!GameObject_isType(go, SMUG_TYPE_BODY));
    GameObject_delete(go);

    go = PositionObject_new();
    PositionObject_setPos(go, 0, 0);
    smug_assert(GameObject_isExactType(go, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(go, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(go, SMUG_TYPE_OBJECT));
    smug_assert(!GameObject_isExactType(go, SMUG_TYPE_BODY));
    smug_assert(!GameObject_isType(go, SMUG_TYPE_BODY));
    GameObject_delete(go);

    go = Drawable_new();
    PositionObject_setPos(go, 3, 4);
    smug_assert(GameObject_isExactType(go, SMUG_TYPE_DRAWABLE));
    smug_assert(GameObject_isType(go, SMUG_TYPE_DRAWABLE));
    smug_assert(GameObject_isType(go, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(go, SMUG_TYPE_OBJECT));
    smug_assert(!GameObject_isExactType(go, SMUG_TYPE_BODY));
    smug_assert(!GameObject_isType(go, SMUG_TYPE_BODY));
    GameObject_delete(go);

    go = DrawableShape_newBox();
    PositionObject_setPos(go, 5, 4);
    smug_assert(GameObject_isExactType(go, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(go, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(go, SMUG_TYPE_DRAWABLE));
    smug_assert(GameObject_isType(go, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(go, SMUG_TYPE_OBJECT));
    smug_assert(!GameObject_isExactType(go, SMUG_TYPE_BODY));
    smug_assert(!GameObject_isType(go, SMUG_TYPE_BODY));
    GameObject_delete(go);

    go = GameObject_newGeneric();
    smug_assert(GameObject_isRootObject(go));
    GameObject* go1 = GameObject_newGeneric();
    smug_assert(GameObject_isRootObject(go1));
    GameObject* go2 = GameObject_newGeneric();
    smug_assert(GameObject_isRootObject(go2));
    GameObject* go3 = GameObject_newGeneric();
    smug_assert(GameObject_isRootObject(go3));
    GameObject_addObject(go, go1);
    GameObject_addObject(go1, go2);
    GameObject_addObject(go2, go3);
    smug_assert(GameObject_isRootObject(go));
    smug_assert(!GameObject_isRootObject(go1));
    smug_assert(!GameObject_isRootObject(go2));
    smug_assert(!GameObject_isRootObject(go3));
    smug_assert(GameObject_nrChildObjects(go) == 1);
    smug_assert(GameObject_nrChildObjects(go1) == 1);
    smug_assert(GameObject_nrChildObjects(go2) == 1);
    smug_assert(GameObject_nrChildObjects(go3) == 0);

    GameObjectIterator* iter = GameObject_getIterator(go, SMUG_TYPE_ANY);
    int count = 0;
    GameObject* goiter;
    smug_assert(GameObjectIterator_hasMore(iter));
    while (GameObjectIterator_hasMore(iter))
    {
        count++;
        goiter = GameObjectIterator_getNext(iter);
        smug_assert(goiter != NULL);
    }
    goiter = GameObjectIterator_getNext(iter);
    smug_assert(goiter == NULL);
    smug_assert(count == 4);
    GameObjectIterator_delete(iter);
    GameObject_delete(go);
    GameObject_delete(go1);
    GameObject_delete(go2);
    GameObject_delete(go3);
}

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
    Engine_addObject((struct GameObject*)obj);
    return 0;
}

SMUGEXPORT SmugObject smugObject_new(void)
{
    return GameObject_newGeneric();
}

SMUGEXPORT SmugObject smugObject_newPositioned(void)
{
    return PositionObject_new();
}

SMUGEXPORT void smugObject_delete(SmugObject object)
{
    GameObject_delete(object);
}

SMUGEXPORT void smugObject_setPos(SmugObject obj, float x, float y)
{
    // TODO: Type checking.
    DO_WRONG_TYPE_ERROR(obj, SMUG_TYPE_POSITION, smugObject_setPos);
    PositionObject_setPos(obj, x, y);
}

SMUGEXPORT void smugObject_moveTo(SmugObject obj, float x, float y)
{
    DO_WRONG_TYPE_ERROR(obj, SMUG_TYPE_POSITION, smugObject_moveTo);
    PositionObject_moveTo(obj, x, y);
}

SMUGEXPORT float smugObject_getX(SmugObject obj)
{
    DO_WRONG_TYPE_ERROR(obj, SMUG_TYPE_POSITION, smugObject_getX);
    float ret;
    BOOL check = PositionObject_getX(obj, &ret);
    if (check)
    {
        return ret;
    }
    else
    {
        // TODO: Do error here.
        return 0.0f;
    }
}

SMUGEXPORT float smugObject_getY(SmugObject obj)
{
    DO_WRONG_TYPE_ERROR(obj, SMUG_TYPE_POSITION, smugObject_getY);
    float ret;
    BOOL check = PositionObject_getY(obj, &ret);
    if (check)
    {
        return ret;
    }
    else
    {
        // TODO: Do error here.
        return 0.0f;
    }
}

SMUGEXPORT void smugObject_addObject(SmugObject obj, SmugObject add)
{
    GameObject_addObject((struct GameObject*)obj, (struct GameObject*)add);
}

SMUGEXPORT void smugObject_addObjectAt(SmugObject obj, SmugObject add, float x, float y)
{
    // TODO: Check type.
    DO_WRONG_TYPE_ERROR(obj, SMUG_TYPE_POSITION, smugObject_addObjectAt);
    DO_WRONG_TYPE_ERROR(add, SMUG_TYPE_POSITION, smugObject_addObjectAt);
    if (0 != PositionObject_addObjectAt(obj, add, x, y))
    {
        WARNING("Could not add positioned object %i to object %i", add, obj);
    }
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
