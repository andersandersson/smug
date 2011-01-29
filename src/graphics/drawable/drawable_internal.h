#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLE_INTERNAL_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLE_INTERNAL_H

#include <graphics/color_type.h>
#include <engine/positionedobject_internal.h>
#include <graphics/drawable/drawable_type.h>

struct BatchData;
struct Sprite;

typedef struct _Drawable
{
    _PositionedObject base; // Inherit from PositionedObject

    Color mColor;
    BOOL mUseColor;
    SmugInheritType mColorInheritance;

    BOOL mVisible;
    SmugInheritType mVisibilityInheritance;

    SmugInheritType mOpacityInheritance;

    int mType;
    unsigned int mLayer;
    struct Sprite* mSprite;
    // unsigned int vertexcount;
    // Vector* vertexOffsets;
    void (*_writeBatchDataFunc)(Drawable* d, struct BatchData* batch, unsigned int start); /**< Function for writing data */
    int (*_getDataSizeFunc)(Drawable* d); /**< Function for getting data size */
    unsigned int (*_getObjectSizeFunc)(Drawable* d); /**< Function for getting data size */
} _Drawable;

#endif /* SMUG_GRAPHICS_DRAWABLE_DRAWABLE_INTERNAL_H */
