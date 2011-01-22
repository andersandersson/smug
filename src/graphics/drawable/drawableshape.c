#include <smugstd.h>

#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/position_object.h>
#include <utils/shapes.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawable_type.h>
#include <graphics/drawable/drawableshape_type.h>

#include <graphics/drawable/drawableshape.h>

struct BatchData;


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
    return FALSE;
    // No faking this one without keeping function pointers in all classes in the hierarchy, i.e., need to call super.function() here.
}

unsigned int _getObjectSize(Drawable* self)
{
    smug_assert(NULL != self);
    return Shape_getNrPoints(((DrawableShape*)self)->mShape);
}

static void _delete(void* data)
{
	DrawableShape_deInit((DrawableShape*)data);
    free((DrawableShape*)data);
}


/*************************/
/** 'Protected' methods **/
/*************************/

void DrawableShape_deInit(DrawableShape* self)
{
	if (self->mShape != NULL)
	{
		Shape_delete(self->mShape);
	}
	Drawable_deInit((Drawable*)self);
}

void DrawableShape_init(DrawableShape* self)
{
	Drawable_init((Drawable*)self);

    ((GameObject*)self)->hasAttribute = _hasAttribute;
    ((GameObject*)self)->inheritAttribute = _inheritAttribute;
    ((GameObject*)self)->deleteMe = _delete;
    ((GameObject*)self)->mTypes |= SMUG_TYPE_SHAPE;

	((Drawable*)self)->_getObjectSizeFunc = _getObjectSize;
}


/**************************/
/** New 'public' methods **/
/**************************/

BOOL DrawableShape_getShape(DrawableShape* self, struct Shape** shape)
{
    *shape = self->mShape;
    return TRUE;
}

BOOL DrawableShape_setShape(DrawableShape* self, struct Shape* shape)
{
    self->mShape = shape;
    return TRUE;
}
