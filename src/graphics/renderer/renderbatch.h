#ifndef SMUG_GRAPHICS_RENDERER_RENDERBATCH_H
#define SMUG_GRAPHICS_RENDERER_RENDERBATCH_H

#include <common/common.h>
#include <utils/linkedlist.h>
#include <graphics/drawable/drawable.h>
#include <graphics/renderer/batchdata.h>

// Holds all data necessary for one render batch

#define BATCHTYPE_POINT 1
#define BATCHTYPE_LINES 2
#define BATCHTYPE_TRIANGLES 3
#define BATCHTYPE_QUADS 4
#define BATCHTYPE_FANS 5

typedef struct RenderBatch
{
    //Texture texture = NULL
    unsigned int objectSize;
    Texture* texture;
    BatchData* data;
    LinkedList* drawables;
    unsigned int dataSize;
    unsigned int vertexBufferIndex;
    unsigned int colorBufferIndex;
    unsigned int textureBufferIndex;
} RenderBatch;

RenderBatch* RenderBatch_new(unsigned int objectSize, Texture* texture, unsigned int initialSize);
void RenderBatch_delete(void* batch);

unsigned int RenderBatch_getDataSize(RenderBatch* batch);

void RenderBatch_render(RenderBatch* batch);
void RenderBatch_write(RenderBatch* batch);

void RenderBatch_addDrawable(RenderBatch* batch, Drawable* drawable);
unsigned int RenderBatch_getDrawableCount(RenderBatch* batch);

#endif // SMUG_GRAPHICS_RENDERER_RENDERBATCH_H
