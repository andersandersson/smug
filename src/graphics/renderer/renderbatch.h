#ifndef SMUG_GRAPHICS_RENDERBATCH_H
#define SMUG_GRAPHICS_RENDERBATCH_H

#include "common/common.h"

// Holds all data necessary for one render batch

#define BATCHTYPE_LOOP 1
#define BATCHTYPE_LINES 2
#define BATCHTYPE_TRIANGLES 3
#define BATCHTYPE_QUADS 4
#define BATCHTYPE_FANS 5


typedef struct RenderBatch
{
    //Texture texture = NULL
    unsigned int objectSize;
    unsigned int _capacity;
    float* vertexData;
    float* colorData;
    float* textureData;
    unsigned int vertexBufferIndex;
    unsigned int colorBufferIndex;
    unsigned int textureBufferIndex;
    
} RenderBatch;

RenderBatch* RenderBatch_new(unsigned int objectSize, unsigned int vertices);
void RenderBatch_delete(void* batch);

void RenderBatch_setCapacity(RenderBatch* batch, unsigned int vertices);
unsigned int RenderBatch_getCapacity(RenderBatch* batch);


#endif //SMUG_GRAPHICS_RENDERBATCH_H
