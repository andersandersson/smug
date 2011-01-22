#include <smugstd.h>

#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/position_object.h>
#include <utils/shapes.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawable_type.h>

#include <graphics/drawable/drawableshape.h>

struct BatchData;

/************************/
/** Convenience macros **/
/************************/

#define _getData(self) ((DrawableShapeData*)InternalGameObject_getData(self, SMUG_TYPE_SHAPE))

/*********************/
/** Internal struct **/
/*********************/

#include <graphics/drawable/drawableshape_type.h>

/******************************************************/
/** 'Overriding' 'inherited' methods from GameObject **/
/******************************************************/

static BOOL _isExactType(GameObject* self, SmugType type)
{
    if (type == SMUG_TYPE_SHAPE)
    {
        return TRUE;
    }
    else
    {
        // smug_printf("Type was not %s, but SMUG_TYPE_SHAPE", InternalGameObject_getTypeString(type));
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
    return FALSE;
    // No faking this one without keeping function pointers in all classes in the hierarchy, i.e., need to call super.function() here.
}

unsigned int _getObjectSize(GameObject* self)
{
    smug_assert(NULL != self);
    return Shape_getNrPoints(_getData(self)->mShape);
}

static void _deleteData(void* data)
{
    Shape_delete(((DrawableShapeData*)data)->mShape);
    free((DrawableShapeData*)data);
}

/*************************/
/** 'Protected' methods **/
/*************************/

InternalGameObject* DrawableShape_newInherit(
    DrawableShapeData* data,
    void(*writeBatchDataFunc)(GameObject* d, struct BatchData* batch, unsigned int start),
    int (*getDataSizeFunc)(GameObject* d))
{
    InternalGameObject* leaf = InternalGameObject_inherit(
        InternalGameObject_getMostSpecific(Drawable_newInherit(writeBatchDataFunc, getDataSizeFunc, _getObjectSize)),
        data);
    leaf->hasAttribute = _hasAttribute;
    leaf->isExactType = _isExactType;
    leaf->inheritAttribute = _inheritAttribute;
    leaf->deleteData = _deleteData;
    return leaf;
}

/**************************/
/** New 'public' methods **/
/**************************/

BOOL DrawableShape_getShape(struct GameObject* self, struct Shape** shape)
{
    DrawableShapeData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *shape = data->mShape;
    return TRUE;
}

BOOL DrawableShape_setShape(struct GameObject* self, struct Shape* shape)
{
    DrawableShapeData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    data->mShape = shape;
    return TRUE;
}
