#include <smugstd.h>

#include <graphics/color.h>
#include <utils/shapes.h>
#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/positionedobject.h>
#include <engine/positionedobject_type.h>

#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawable_type.h>

struct BatchData;
struct Sprite;


#define _isColorInheritType(type) (type == SMUG_COLOR_INHERIT || type == SMUG_COLOR_KEEP || type == SMUG_COLOR_BLEND)
#define _isOpacityInheritType(type) (type == SMUG_OPACITY_INHERIT || type == SMUG_OPACITY_KEEP)
#define _isVisibilityInheritType(type) (type == SMUG_VISIBILITY_INHERIT || type == SMUG_VISIBILITY_KEEP)


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
    switch (attr)
    {
        case SMUG_ATTR_COLOR:
            if (_isColorInheritType(type))
            {
                ((Drawable*)self)->mColorInheritance = type;
                return TRUE;
            }
            return FALSE;
        case SMUG_ATTR_OPACITY:
            if (_isOpacityInheritType(type))
            {
                ((Drawable*)self)->mOpacityInheritance = type;
                return TRUE;
            }
            return FALSE;
        case SMUG_ATTR_VISIBILITY:
            if (_isVisibilityInheritType(type))
            {
                ((Drawable*)self)->mVisibilityInheritance = type;
                return TRUE;
            }
            return FALSE;
        default:
            return FALSE;
    }
}

static void _delete(void* self)
{
    Drawable_deInit((Drawable*)self);
    free((Drawable*)self);
}


/***************************/
/** 'Protected' functions **/
/***************************/

void Drawable_deInit(Drawable* self)
{
    self->mColorInheritance = SMUG_INHERIT_UNDEFINED;
    self->mVisibilityInheritance = SMUG_INHERIT_UNDEFINED;
    self->mOpacityInheritance = SMUG_INHERIT_UNDEFINED;
    self->mType = 0;
    self->mLayer = 0;
    self->_writeBatchDataFunc = NULL;
    self->_getDataSizeFunc = NULL;
    self->_getObjectSizeFunc = NULL;
    PositionedObject_deInit((PositionedObject*)self);
}

void Drawable_init(Drawable* self)
{
    PositionedObject_init((PositionedObject*)self);
    ((GameObject*)self)->hasAttribute = _hasAttribute;
    ((GameObject*)self)->inheritAttribute = _inheritAttribute;
    ((GameObject*)self)->deleteMe = _delete;
    ((GameObject*)self)->mTypes |= SMUG_TYPE_DRAWABLE;

    self->mColor = Color_createFromRGBAi(0, 0, 0, 0);
    self->mColorInheritance = SMUG_COLOR_INHERIT;
    self->mVisible = TRUE;
    self->mVisibilityInheritance = SMUG_VISIBILITY_INHERIT;
    self->mOpacity = 1.0f;
    self->mOpacityInheritance = SMUG_OPACITY_INHERIT;
    self->mType = 0;  // TODO: proper value, whatever that is.
    self->mLayer = 0;
    self->_writeBatchDataFunc = NULL;
    self->_getDataSizeFunc = NULL;
    self->_getObjectSizeFunc = NULL;
}


/**********************/
/** public functions **/
/**********************/

Drawable* Drawable_newGeneric(void)
{
    Drawable* newObj = (Drawable*)malloc(sizeof(Drawable*));
    Drawable_init(newObj);
    return newObj;
}

void Drawable_writeBatchData(Drawable* self, struct BatchData* batchdata, unsigned int start)
{
    smug_assert(NULL != self);
    self->_writeBatchDataFunc(self, batchdata, start);
}

int Drawable_getDataSize(Drawable* self)
{
    smug_assert(NULL != self);
    return self->_getDataSizeFunc(self);
}

void Drawable_setLayer(Drawable* self, unsigned int layer)
{
    smug_assert(NULL != self);
    self->mLayer = layer;
}

unsigned int Drawable_getLayer(Drawable* self)
{
    smug_assert(NULL != self);
    return self->mLayer;
}

unsigned int Drawable_getObjectSize(Drawable* self)
{
    smug_assert(NULL != self);
    return self->_getObjectSizeFunc(self);
}

struct Sprite* Drawable_getSprite(Drawable* self)
{
    return NULL;
}

BOOL Drawable_setColor(Drawable* self, Color c)
{
    self->mColor = c;
    return TRUE;
}

BOOL Drawable_getColor(Drawable* self, Color* c)
{
    *c = self->mColor;
    return TRUE;
}

BOOL Drawable_setOpacity(Drawable* self, float opacity)
{
    self->mOpacity = opacity;
    return TRUE;
}

BOOL Drawable_getOpacity(Drawable* self, float* opacity)
{
    *opacity = self->mOpacity;
    return TRUE;
}

struct Texture* Drawable_getTexture(struct Drawable* d)
{
    return NULL;
}

unsigned int Drawable_getTextureID(struct Drawable* d)
{
    return 0;
}
