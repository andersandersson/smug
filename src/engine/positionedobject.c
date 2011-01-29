#include <smugstd.h>

#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/interpoint.h>
#include <utils/point.h>
#include <utils/vector_type.h>

#include <engine/positionedobject_type.h>
#include <engine/positionedobject_internal.h>
#include <engine/positionedobject.h>


#define _isPositionInheritType(type) (type == SMUG_POSITION_INHERIT || type == SMUG_POSITION_KEEP || type == SMUG_POSITION_RELATIVE)


/*************************************************/
/** Local methods overriding ones in GameObject **/
/*************************************************/

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
            ((PositionedObject*)self)->mPositionInheritance = type;
            return TRUE;
        }
    }
    return FALSE;
}

static void _delete(void* self)
{
    PositionedObject_deInit((PositionedObject*)self);
    free((PositionedObject*)self);
}


/*************************/
/** protected functions **/
/*************************/

void PositionedObject_deInit(PositionedObject* self)
{
    Interpoint_delete(self->mPosition);
    self->mPosition = NULL;
    self->mPositionInheritance = SMUG_INHERIT_UNDEFINED;
    GameObject_deInit((GameObject*)self);
}

void PositionedObject_init(PositionedObject* self)
{
    GameObject_init((GameObject*)self);

    ((GameObject*)self)->mTypes |= SMUG_TYPE_POSITIONED;
    ((GameObject*)self)->hasAttribute = _hasAttribute;
    ((GameObject*)self)->inheritAttribute = _inheritAttribute;
    ((GameObject*)self)->deleteMe = _delete;

    self->mPosition = Interpoint_new(Point_createFromXY(0,0));
    self->mPositionInheritance = SMUG_POSITION_RELATIVE;
}


/**************************/
/** New 'public' methods **/
/**************************/

PositionedObject* PositionedObject_new(void)
{
    PositionedObject* newObj = (PositionedObject*)malloc(sizeof(PositionedObject));
    PositionedObject_init(newObj);
    return newObj;
}

BOOL PositionedObject_setPos(PositionedObject* self, float x, float y)
{
    // smug_printf("Setting object position to %f, %f", x, y);
    Interpoint_setTo(self->mPosition, Point_createFromXY(x, y));
    return TRUE;
}

BOOL PositionedObject_getPos(PositionedObject* self, Vector* v)
{
    *v = Point_getVector(Interpoint_getPoint(self->mPosition));
    return TRUE;
}

BOOL PositionedObject_getX(PositionedObject* self, float* x)
{
    *x = Point_getX(Interpoint_getPoint(self->mPosition));
    return TRUE;
}

BOOL PositionedObject_getY(PositionedObject* self, float* y)
{
    *y = Point_getY(Interpoint_getPoint(self->mPosition));
    return TRUE;
}

BOOL PositionedObject_moveTo(PositionedObject* self, float x, float y)
{
    // smug_printf("Moving object to %f, %f", x, y);
    Interpoint_moveTo(self->mPosition, Point_createFromXY(x, y));
    return TRUE;
}

BOOL PositionedObject_getPosForDrawing(GameObject* self, Point* p)
{
    if (GameObject_isType(self, SMUG_TYPE_POSITIONED))
    {
        Point myPos = Interpoint_getInterpolated(((PositionedObject*)self)->mPosition);
        if (GameObject_isRootObject(self) || ((PositionedObject*)self)->mPositionInheritance == SMUG_POSITION_KEEP)
        {
            *p = myPos;
            return TRUE;
        }
        else
        {
            Point pp = Point_createFromXY(0, 0);
            PositionedObject_getPosForDrawing(GameObject_getParent(self), &pp);
            if (((PositionedObject*)self)->mPositionInheritance == SMUG_POSITION_INHERIT)
            {
                *p = pp;
                return TRUE;
            }
            else
            {
                smug_assert(((PositionedObject*)self)->mPositionInheritance == SMUG_POSITION_RELATIVE);
                *p = Point_add(myPos, pp);
                return TRUE;
            }
        }
    }
    else
    {
        if (GameObject_isRootObject(self))
        {
            return FALSE;
        }
        else
        {
            return PositionedObject_getPosForDrawing(GameObject_getParent(self), p);
        }
    }
/*     if (self->mPositionInheritance == SMUG_POSITION_RELATIVE
        && !GameObject_isRootObject((GameObject*)self)
        && GameObject_isType(GameObject_getParent((GameObject*)self), SMUG_TYPE_POSITIONED))
    {
        Point pp;
        PositionedObject_getPosForDrawing((PositionedObject*)GameObject_getParent((GameObject*)self), &pp);
        Point p1 = Interpoint_getInterpolated(self->mPosition);
        // smug_printf("Object %i has pos (%i) %f, %f relative to %f, %f", self, data->mPosition, Point_getX(p1), Point_getY(p1), Point_getX(pp), Point_getY(pp));
        *p = Point_add(p1, pp);
    }
    else
    {
        *p = Interpoint_getInterpolated(self->mPosition);
        // smug_printf("Object %i has pos %f, %f", self, Point_getX(*p), Point_getY(*p));
    }
    return TRUE; */
}

BOOL PositionedObject_commitPosition(PositionedObject* self)
{
    Interpoint_commit(self->mPosition);
    return TRUE;
}

int PositionedObject_addObjectAt(PositionedObject* self, PositionedObject* other, float x, float y)
{
    if (0 != GameObject_addObject((GameObject*)self, (GameObject*)other))
    {
        return 1;
    }
    PositionedObject_setPos(other, x, y);
    // smug_assert(Point_equal(Interpoint_getPoint(data->mPosition), Point_createFromXY(x, y)));
    other->mPositionInheritance = SMUG_POSITION_RELATIVE;
    return 0;
}
