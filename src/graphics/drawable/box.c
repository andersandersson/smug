#include "box.h"
#include <stdlib.h>

Box* Box_new()
{
    Box* ret = (Box*)malloc(sizeof(Box));
    ret->base._deleteFunc = Box_delete;
    ret->base._writeBatchFunc = Box_writeBatchData;
    ret->base.layer = 0;
    ret->rect.x = 0;
    ret->rect.y = 0;
    ret->rect.w = 0;    
    ret->rect.h = 0;    
    
    return ret;
}

Box* Box_newFromData(float x, float y, float w, float h)
{
    Box* ret = (Box*)malloc(sizeof(Box));
    ret->base._deleteFunc = Box_delete;
    ret->base._writeBatchFunc = Box_writeBatchData;
    ret->base.layer = 0;
    ret->rect.x = x;
    ret->rect.y = y;
    ret->rect.w = w;    
    ret->rect.h = h;   
    
    return ret;
}

void Box_delete(void* box)
{
    if (NULL != box)
    {
        free((Box*)box);
    }
}

int Box_writeBatchData(Drawable* box, RenderBatch* batch, unsigned int start)
{
    unsigned int vertexstart = start*2;
    unsigned int colorstart = start*4; 
    unsigned int texturestart = start*2; 
    Box* thebox = (Box*)box;
    
    float x1 = thebox->rect.x;
    float x2 = thebox->rect.x + thebox->rect.w;
    float y1 = thebox->rect.y;
    float y2 = thebox->rect.y + thebox->rect.h;
   
    // write vertices in anti-clockwise order
    batch->vertexData[vertexstart + 0 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 0 * 2 + 1] = y1;
    batch->vertexData[vertexstart + 1 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 1 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 2 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 2 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 3 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 3 * 2 + 1] = y1;   
    
    float r = thebox->color.r;
    float g = thebox->color.g;
    float b = thebox->color.b;
    float a = thebox->color.a;
    
    // write colordata
    batch->colorData[colorstart + 0 * 4 + 0] = r;
    batch->colorData[colorstart + 0 * 4 + 1] = g;
    batch->colorData[colorstart + 0 * 4 + 2] = b;
    batch->colorData[colorstart + 0 * 4 + 3] = a; 
    batch->colorData[colorstart + 1 * 4 + 0] = r;
    batch->colorData[colorstart + 1 * 4 + 1] = g;
    batch->colorData[colorstart + 1 * 4 + 2] = b;
    batch->colorData[colorstart + 1 * 4 + 3] = a;  
    batch->colorData[colorstart + 2 * 4 + 0] = r;
    batch->colorData[colorstart + 2 * 4 + 1] = g;
    batch->colorData[colorstart + 2 * 4 + 2] = b;
    batch->colorData[colorstart + 2 * 4 + 3] = a;    
    batch->colorData[colorstart + 3 * 4 + 0] = r;
    batch->colorData[colorstart + 3 * 4 + 1] = g;
    batch->colorData[colorstart + 3 * 4 + 2] = b;
    batch->colorData[colorstart + 3 * 4 + 3] = a;   
    
    // leave texture data
    //
    
      
    // number of vertices written
    return 4;
}