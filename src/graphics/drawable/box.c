#include "box.h"
#include <stdlib.h>

Box* Box_new()
{
    Box* ret = (Box*)malloc(sizeof(Box));
    ret->base._deleteFunc = Box_delete;
    ret->base._writeBatchFunc = Box_writeBatchData;
    ret->base.layer = 0;
    ret->base.pos = Point_createFromXY(0, 0);
    ret->rect = Rectangle_createFromXYWH(0,0,0,0);
    
    return ret;
}

Box* Box_newFromPointRectangle(Point pos, Rectangle rect)
{
    Box* ret = (Box*)malloc(sizeof(Box));
    ret->base._deleteFunc = Box_delete;
    ret->base._writeBatchFunc = Box_writeBatchData;
    ret->base.layer = 0;
    ret->base.pos = pos;
    ret->rect = rect;
    
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
    static unsigned int vertexstart, colorstart, texturestart;
    static float x1, x2, y1, y2;
    static float r, g, b, a;

    vertexstart = start*2;
    colorstart = start*4; 
    texturestart = start*2; 
    Box* thebox = (Box*)box;
    
    x1 = Point_getX(&thebox->base.pos) + Rectangle_getX(&thebox->rect);
    x2 = Point_getX(&thebox->base.pos) + Rectangle_getX(&thebox->rect) + Rectangle_getW(&thebox->rect);
    y1 = Point_getY(&thebox->base.pos) + Rectangle_getY(&thebox->rect);
    y2 = Point_getY(&thebox->base.pos) + Rectangle_getY(&thebox->rect) + Rectangle_getH(&thebox->rect);
   
    // write vertices in anti-clockwise order
    batch->vertexData[vertexstart + 0 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 0 * 2 + 1] = y1;
    batch->vertexData[vertexstart + 1 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 1 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 2 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 2 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 3 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 3 * 2 + 1] = y1;   
    
    r = thebox->color.r;
    g = thebox->color.g;
    b = thebox->color.b;
    a = thebox->color.a;
    
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