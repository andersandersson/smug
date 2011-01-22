#include <smugstd.h>

#include <graphics/color.h>
#include <utils/shapes.h>
#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/position_object.h>
#include <graphics/drawable/drawable.h>

#include <graphics/drawable/drawable_type.h>

struct BatchData;
struct Sprite;

/************************/
/** Convenience macros **/
/************************/

#define _isColorInheritType(type) (type == SMUG_COLOR_INHERIT || type == SMUG_COLOR_KEEP || type == SMUG_COLOR_BLEND)
#define _isOpacityInheritType(type) (type == SMUG_OPACITY_INHERIT || type == SMUG_OPACITY_KEEP)
#define _isVisibilityInheritType(type) (type == SMUG_VISIBILITY_INHERIT || type == SMUG_VISIBILITY_KEEP)

#define _getData(self) ((DrawableObjectData*)InternalGameObject_getData(self, SMUG_TYPE_DRAWABLE))

/******************************************************/
/** 'Overriding' 'inherited' methods from GameObject **/
/******************************************************/

static BOOL _isExactType(GameObject* self, SmugType type)
{
    if (type == SMUG_TYPE_DRAWABLE)
    {
        return TRUE;
    }
    else
    {
        // smug_printf("Type was not %s, but SMUG_TYPE_DRAWABLE", InternalGameObject_getTypeString(type));
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
    switch (attr)
    {
        case SMUG_ATTR_COLOR:
            if (_isColorInheritType(type))
            {
                _getData(self)->mColorInheritance = type;
                return TRUE;
            }
            return FALSE;
        case SMUG_ATTR_OPACITY:
            if (_isOpacityInheritType(type))
            {
                _getData(self)->mOpacityInheritance = type;
                return TRUE;
            }
            return FALSE;
        case SMUG_ATTR_VISIBILITY:
            if (_isVisibilityInheritType(type))
            {
                _getData(self)->mVisibilityInheritance = type;
                return TRUE;
            }
            return FALSE;
        default:
            return FALSE;
    }
}

static void _deleteData(void* self)
{
    free((DrawableObjectData*)self);
}

/**************************/
/** Local helper methods **/
/**************************/

static DrawableObjectData* DrawableObjectData_new(void)
{
    DrawableObjectData* newObj = malloc(sizeof(DrawableObjectData));
    newObj->mColor = Color_createFromRGBAi(0, 0, 0, 0);
    newObj->mColorInheritance = SMUG_COLOR_INHERIT;
    newObj->mVisible = TRUE;
    newObj->mVisibilityInheritance = SMUG_VISIBILITY_INHERIT;
    newObj->mOpacity = 1.0f;
    newObj->mOpacityInheritance = SMUG_OPACITY_INHERIT;
    newObj->mType = 0;  // TODO: proper value, whatever that is.
    newObj->mLayer = 0;
    return newObj;
}

/*************************/
/** 'Protected' methods **/
/*************************/

GameObject* Drawable_newInherit(
    void(*writeBatchDataFunc)(GameObject* d, struct BatchData* batch, unsigned int start),
    int (*getDataSizeFunc)(GameObject* d),
    unsigned int (*getObjectSizeFunc)(struct GameObject* d))
{
    GameObject* obj = Drawable_new();
    DrawableObjectData* data = _getData(obj);
    data->_writeBatchDataFunc = writeBatchDataFunc;
    data->_getDataSizeFunc = getDataSizeFunc;
    data->_getObjectSizeFunc = getObjectSizeFunc;
    return obj;
}


/**************************/
/** New 'public' methods **/
/**************************/

GameObject* Drawable_new()
{
    InternalGameObject* leaf = InternalGameObject_inherit(InternalGameObject_getMostSpecific(PositionObject_new()), DrawableObjectData_new());
    leaf->hasAttribute = _hasAttribute;
    leaf->isExactType = _isExactType;
    leaf->inheritAttribute = _inheritAttribute;
    leaf->deleteData = _deleteData;
    return InternalGameObject_getAsGeneric(leaf);
}

void Drawable_writeBatchData(GameObject* self, struct BatchData* batchdata, unsigned int start)
{
    smug_assert(NULL != self);
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        smug_assert(FALSE);
        return;
    }
    data->_writeBatchDataFunc(self, batchdata, start);
}

int Drawable_getDataSize(GameObject* self)
{
    smug_assert(NULL != self);
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        smug_assert(FALSE);
        return 0;
    }
    return data->_getDataSizeFunc(self);
}

void Drawable_setLayer(GameObject* self, unsigned int layer)
{
    smug_assert(NULL != self);
    _getData(self)->mLayer = layer;
}

unsigned int Drawable_getLayer(GameObject* self)
{
    smug_assert(NULL != self);
    return _getData(self)->mLayer;
}

unsigned int Drawable_getObjectSize(GameObject* self)
{
    smug_assert(NULL != self);
    smug_assert(_getData(self) != NULL);
    return _getData(self)->_getObjectSizeFunc(self);
}

struct Sprite* Drawable_getSprite(GameObject* self)
{
    return NULL;
}

BOOL Drawable_setColor(GameObject* self, Color c)
{
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    data->mColor = c;
    return TRUE;
}

BOOL Drawable_getColor(GameObject* self, Color* c)
{
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *c = data->mColor;
    return TRUE;
}

BOOL Drawable_setOpacity(GameObject* self, float opacity)
{
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    data->mOpacity = opacity;
    return TRUE;
}

BOOL Drawable_getOpacity(GameObject* self, float* opacity)
{
    DrawableObjectData* data = _getData(self);
    if (data == NULL)
    {
        return FALSE;
    }
    *opacity = data->mOpacity;
    return TRUE;
}

struct Texture* Drawable_getTexture(struct GameObject* d)
{
    return NULL;
}

unsigned int Drawable_getTextureID(struct GameObject* d)
{
    return 0;
}
