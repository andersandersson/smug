#ifndef SMUG_GRAPHICS_RENDERER_BATCHDATA_H
#define SMUG_GRAPHICS_RENDERER_BATCHDATA_H

typedef struct BatchData
{
    unsigned int capacity;
    float* vertexData;
    float* colorData;
    float* textureData;
} BatchData;

BatchData* BatchData_new(unsigned int initialSize);
void BatchData_delete(void* data);

void BatchData_setCapacity(BatchData* batch, unsigned int size);
void BatchData_growCapacityTo(BatchData* batch, unsigned int size);
unsigned int BatchData_getCapacity(BatchData* batch);
void BatchData_expandTo(BatchData* batch, unsigned int size);

//void BatchData_freeData(BatchData* data);
//void BatchData_setSize(BatchData* data, unsigned int size);
//void BatchData_copyData(BatchData* todata, Batchdata* fromdata, unsigned int size);

#endif // SMUG_GRAPHICS_RENDERER_BATCHDATA_H
