#include <smugstd.h>

#include <graphics/color.h>
#include <utils/shape.h>
#include <utils/vector.h>
#include <utils/rectangle.h>
#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/positionedobject.h>
#include <engine/positionedobject_type.h>
#include <graphics/sprite.h>
#include <graphics/texture/texture.h>

#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawable_internal.h>

struct BatchData;
struct Sprite;


#define _isColorInheritType(type) (type == SMUG_COLOR_INHERIT || type == SMUG_COLOR_KEEP || type == SMUG_COLOR_BLEND)
#define _isOpacityInheritType(type) (type == SMUG_OPACITY_INHERIT || type == SMUG_OPACITY_KEEP)
#define _isVisibilityInheritType(type) (type == SMUG_VISIBILITY_INHERIT || type == SMUG_VISIBILITY_KEEP)


BOOL _invariant(Drawable* self)
{
    smug_assert(self != NULL);
    smug_assert(self->mShape != NULL);
    return self != NULL && self->mShape != NULL;
}

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
    if (self->mShape != NULL)
    {
        Shape_delete(self->mShape);
    }
    if (self->mSprite != NULL)
    {
        Sprite_delete(self->mSprite);
    }
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

    self->mAnchorPoint = Vector_create2d(0, 0);
    self->mColor = Color_createFromRGBAi(0, 0, 0, 255);
    self->mUseColor = FALSE;
    self->mOpacityInheritance = SMUG_OPACITY_INHERIT;
    self->mColorInheritance = SMUG_COLOR_INHERIT;
    self->mShape = NULL;
    self->mSprite = NULL;

    self->mVisible = TRUE;
    self->mVisibilityInheritance = SMUG_VISIBILITY_INHERIT;
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
    smug_assert(_invariant(self));
    return self->mSprite;
}

void Drawable_setSprite(Drawable* self, struct Sprite* sprite)
{
    smug_assert(_invariant(self));
    self->mSprite = sprite;
    self->mUseColor = FALSE;
}

BOOL Drawable_setColor(Drawable* self, Color c)
{
    self->mColor = c;
    self->mUseColor = TRUE;
    return TRUE;
}

BOOL Drawable_getColor(Drawable* self, Color* c)
{
    *c = self->mColor;
    return TRUE;
}

BOOL Drawable_getUseColor(Drawable* self, BOOL* use)
{
    *use = self->mUseColor;
    return TRUE;
}

BOOL Drawable_setUseColor(Drawable* self, BOOL use)
{
    self->mUseColor = use;
    return TRUE;
}

BOOL Drawable_setOpacity(Drawable* self, float opacity)
{
    // self->mOpacity = opacity;
    self->mColor = Color_setCompAf(self->mColor, opacity);
    return TRUE;
}

BOOL Drawable_getOpacity(Drawable* self, float* opacity)
{
    *opacity = Color_Af(self->mColor);
    return TRUE;
}

BOOL Drawable_getShape(Drawable* self, Shape** shape)
{
    *shape = self->mShape;
    return TRUE;
}

void Drawable_setAnchorPoint(Drawable* self, Vector point)
{
    smug_assert(_invariant(self));
    if (self->mShape != NULL)
    {
        Rectangle rect = Shape_getAsRectangle(self->mShape);
        smug_printf("Moving shape by vector. Before: (%f, %f, %f, %f)", Rectangle_getX(&rect), Rectangle_getY(&rect), Rectangle_getW(&rect), Rectangle_getH(&rect));
        smug_printf("New vector: (%f, %f)", Vector_getX(&point), Vector_getY(&point));
        smug_printf("Old vector: (%f, %f)", Vector_getX(&self->mAnchorPoint), Vector_getY(&self->mAnchorPoint));
        Vector v = Vector_sub(point, self->mAnchorPoint);
        smug_printf("Moving by vector: (%f, %f)", Vector_getX(&v), Vector_getY(&v));
        v = Vector_multiply(v, -1.0);
        smug_printf("Moving by negative vector: (%f, %f)", Vector_getX(&v), Vector_getY(&v));
        Shape_moveByVector(self->mShape, v);
        rect = Shape_getAsRectangle(self->mShape);
        smug_printf("Moved shape by vector. After: (%f, %f, %f, %f)", Rectangle_getX(&rect), Rectangle_getY(&rect), Rectangle_getW(&rect), Rectangle_getH(&rect));
        self->mAnchorPoint = point;
    }
}

Vector Drawable_getAnchorPoint(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->mAnchorPoint;
}

void Drawable_scale(Drawable* self, float scale)
{
    smug_assert(_invariant(self));
    if (self->mShape != NULL)
    {
        Shape_scale(self->mShape, scale);
    }
}

void Drawable_scaleX(Drawable* self, float scale)
{
    smug_assert(_invariant(self));
    if (self->mShape != NULL)
    {
        Shape_scaleX(self->mShape, scale);
    }
}

void Drawable_scaleY(Drawable* self, float scale)
{
    smug_assert(_invariant(self));
    if (self->mShape != NULL)
    {
        Shape_scaleY(self->mShape, scale);
    }
}

void Drawable_scaleXY(Drawable* self, Vector scales)
{
    smug_assert(_invariant(self));
    if (self->mShape != NULL)
    {
        Shape_scaleXY(self->mShape, scales);
    }
}

Texture* Drawable_getTexture(Drawable* self)
{
    smug_assert(self != NULL);
    if (self->mSprite == NULL)
    {
        return NULL;
    }
    smug_assert(self->mSprite != NULL);
    return Sprite_getTexture(self->mSprite);
}

unsigned int Drawable_getTextureID(Drawable* self)
{
    smug_assert(self != NULL);
    if (self->mSprite == NULL)
    {
        return 0;
    }
    smug_assert(self->mSprite != NULL);
    return Sprite_getTextureId(self->mSprite);
}
