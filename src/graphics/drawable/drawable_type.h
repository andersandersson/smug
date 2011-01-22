#include <graphics/color_type.h>
struct GameObject;
struct _PositionObject;
struct BatchData;

typedef struct DrawableObjectData
{
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
    void (*_writeBatchDataFunc)(struct GameObject* d, struct BatchData* batch, unsigned int start); /**< Function for writing data */
    int (*_getDataSizeFunc)(struct GameObject* d); /**< Function for getting data size */
    unsigned int (*_getObjectSizeFunc)(struct GameObject* d); /**< Function for getting data size */

    // void* mData; // For subtypes, shape, image, etc.
    // struct _PositionObject* mParent;

} DrawableObjectData;
