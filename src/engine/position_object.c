#include <smugstd.h>

#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/position_object.h>
#include <engine/interpoint.h>
#include <utils/point.h>
#include <utils/vector_type.h>

#include <engine/position_object_type.h>

/************************/
/** Convenience macros **/
/************************/

#define _isPositionInheritType(type) (type == SMUG_POSITION_INHERIT || type == SMUG_POSITION_KEEP || type == SMUG_POSITION_RELATIVE)

// Returns the PositionObjectData associated with an object, or NULL if the object is not a PositionObject.
#define _getData(self) ((PositionObjectData*)InternalGameObject_getData(self, SMUG_TYPE_POSITION))

/******************************************************/
/** 'Overriding' 'inherited' methods from GameObject **/
/******************************************************/

static BOOL _isExactType(GameObject* self, SmugType type)
{
    if (type == SMUG_TYPE_POSITION)
    {
        return TRUE;
    }
    else
    {
        // smug_printf("Type was not %s, but SMUG_TYPE_POSITION", InternalGameObject_getTypeString(type));
        return FALSE;
    }
}

static BOOL _hasAttribute(GameObject* self, SmugAttribute attr)
{
    return  attr == SMUG_ATTR_COLOR ||
            attr == SMUG_ATTR_OPACITY ||
            attr == SMUG_ATTR_VISIBILITY;
}

static BOOL _inheritAttribute(GameObject* self, SmugAttribute attr, SmugInheritType type)
{
    if (attr == SMUG_ATTR_POSITION)
    {
        if (_isPositionInheritType(type))
        {
            _getData(self)->mPositionInheritance = type;
            return TRUE;
        }
    }
    return FALSE;
}

static void _deleteData(void* data)
{
    // smug_printf("Deleting position object.");
    Interpoint_delete(((PositionObjectData*)data)->mPosition);
    // Interpoint_delete(((PositionObjectData*)data)->mRelativePos);
    free(data);
}

/**************************/
/** Local helper methods **/
/**************************/

static PositionObjectData* PositionObjectData_new(void)
{
    PositionObjectData* data = (PositionObjectData*)malloc(sizeof(PositionObjectData));
    data->mPosition = Interpoint_new(Point_createFromXY(0,0));
    // data->mRelativePos = Interpoint_new(Point_createFromXY(0,0));
    data->mPositionInheritance = SMUG_POSITION_RELATIVE;
    return data;
}

/**************************/
/** New 'public' methods **/
/**************************/

GameObject* PositionObject_new()
{
    InternalGameObject* leaf = InternalGameObject_inherit(GameObject_newGeneric(), PositionObjectData_new());
    leaf->hasAttribute = _hasAttribute;
    leaf->isExactType = _isExactType;
    leaf->inheritAttribute = _inheritAttribute;
    leaf->deleteData = _deleteData;
    return InternalGameObject_getAsGeneric(leaf);
}

BOOL PositionObject_setPos(GameObject* self, float x, float y)
{
    // smug_printf("Setting object position to %f, %f", x, y);
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    Interpoint_setTo(data->mPosition, Point_createFromXY(x, y));
    return TRUE;
}

BOOL PositionObject_getPos(GameObject* self, Vector* v)
{
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *v = Point_getVector(Interpoint_getPoint(data->mPosition));
    return TRUE;
}

BOOL PositionObject_getX(GameObject* self, float* x)
{
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *x = Point_getX(Interpoint_getPoint(data->mPosition));
    return TRUE;
}

BOOL PositionObject_getY(GameObject* self, float* y)
{
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *y = Point_getY(Interpoint_getPoint(data->mPosition));
    return TRUE;
}

BOOL PositionObject_moveTo(GameObject* self, float x, float y)
{
    // smug_printf("Moving object to %f, %f", x, y);
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    Interpoint_moveTo(data->mPosition, Point_createFromXY(x, y));
    return TRUE;
}

BOOL PositionObject_getPosForDrawing(GameObject* self, Point* p)
{
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    if (data->mPositionInheritance == SMUG_POSITION_RELATIVE
        && !GameObject_isRootObject(self)
        && GameObject_isType(GameObject_getParent(self), SMUG_TYPE_POSITION))
    {
        Point pp;
        PositionObject_getPosForDrawing(GameObject_getParent(self), &pp);
        Point p1 = Interpoint_getInterpolated(data->mPosition);
        // Point p1 = Interpoint_getPoint(data->mPosition);
        // smug_printf("Object %i has pos (%i) %f, %f relative to %f, %f", self, data->mPosition, Point_getX(p1), Point_getY(p1), Point_getX(pp), Point_getY(pp));
        *p = Point_add(p1, pp);
    }
    else
    {
        // smug_printf("Object %i did not inherit position.", self);
        *p = Interpoint_getInterpolated(data->mPosition);
        // *p = Interpoint_getPoint(data->mPosition);
        // smug_printf("Object %i has pos %f, %f", self, Point_getX(*p), Point_getY(*p));
    }
    return TRUE;
}

BOOL PositionObject_commitPosition(GameObject* self)
{
    PositionObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    Interpoint_commit(data->mPosition);
    return TRUE;
}

int PositionObject_addObjectAt(struct GameObject* self, struct GameObject* other, float x, float y)
{
    if (0 != GameObject_addObject(self, other))
    {
        return 1;
    }
    PositionObject_setPos(other, x, y);
    PositionObjectData* data = _getData(self);
    // Interpoint_setTo(data->mPosition, Point_createFromXY(x, y));
    // smug_assert(Point_equal(Interpoint_getPoint(data->mPosition), Point_createFromXY(x, y)));
    // smug_printf("Added positioned object %i to object %i at %f, %f", other, self, Point_getX(Point_createFromXY(x, y)), Point_getY(Point_createFromXY(x, y)));
    data->mPositionInheritance = SMUG_POSITION_RELATIVE;
    return 0;
}