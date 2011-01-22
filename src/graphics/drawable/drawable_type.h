#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLE_TYPE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLE_TYPE_H

#include <graphics/color_type.h>
#include <engine/position_object_type.h>
struct GameObject;
struct _PositionObject;
struct BatchData;

typedef struct Drawable
{
    PositionedObject base; // Inherit from PositionedObject

    Color mColor;
    SmugInheritType mColorInheritance;

    BOOL mVisible;
    SmugInheritType mVisibilityInheritance;

    float mOpacity;
    SmugInheritType mOpacityInheritance;

    int mType;
    unsigned int mLayer;
    // Sprite* sprite; // Is NULL for shapes
    // unsigned int vertexcount;
    // Vector* vertexOffsets;
    void (*_writeBatchDataFunc)(struct Drawable* d, struct BatchData* batch, unsigned int start); /**< Function for writing data */
    int (*_getDataSizeFunc)(struct Drawable* d); /**< Function for getting data size */
    unsigned int (*_getObjectSizeFunc)(struct Drawable* d); /**< Function for getting data size */
} Drawable;

#endif /* SMUG_GRAPHICS_DRAWABLE_DRAWABLE_TYPE_H */
