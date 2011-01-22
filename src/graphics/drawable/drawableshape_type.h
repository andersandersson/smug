struct _DrawableObject;
struct Shape;

typedef struct DrawableShapeData
{
    struct Shape* mShape;
    // Can shape be inherited???

    // int type;
    // unsigned int mLayer;

    // void (*_writeBatchDataFunc)(GameObject* d, BatchData* batch, unsigned int start); /**< Function for writing data */
    // int (*_getDataSizeFunc)(GameObject* d); /**< Function for getting data size */

    // void* mData; // For subtypes.
    // struct _DrawableObject* mParent;

} DrawableShapeData;
