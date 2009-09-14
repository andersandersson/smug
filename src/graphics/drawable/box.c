#include "box.h"
#include <stdlib.h>

static int getDataSize(Drawable* drawable)
{
    // number of vertices that shall be written
    return 4;
}

static void writeBatchData(Drawable* drawable, BatchData* batchdata, unsigned int start)
{
    static unsigned int vertexstart, colorstart, texturestart;
    static float x1, x2, y1, y2;
    static float r, g, b, a;
    static float tx1, ty1, tx2, ty2;
    static Sprite* sprite = NULL;
    
    vertexstart = start*2;
    colorstart = start*4; 
    texturestart = start*2; 
   
    // write vertices in anti-clockwise order
    x1 = Point_getX(&drawable->pos) + drawable->vertices[0].d[0];
    x2 = Point_getX(&drawable->pos) + drawable->vertices[1].d[0];
    y1 = Point_getY(&drawable->pos) + drawable->vertices[0].d[1];
    y2 = Point_getY(&drawable->pos) + drawable->vertices[1].d[1];
   
    batchdata->vertexData[vertexstart + 0 * 2 + 0] = x1;
    batchdata->vertexData[vertexstart + 0 * 2 + 1] = y1;
    batchdata->vertexData[vertexstart + 1 * 2 + 0] = x1;
    batchdata->vertexData[vertexstart + 1 * 2 + 1] = y2; 
    batchdata->vertexData[vertexstart + 2 * 2 + 0] = x2;
    batchdata->vertexData[vertexstart + 2 * 2 + 1] = y2; 
    batchdata->vertexData[vertexstart + 3 * 2 + 0] = x2;
    batchdata->vertexData[vertexstart + 3 * 2 + 1] = y1;   
 
    // write colordata   
    r = drawable->color.r;
    g = drawable->color.g;
    b = drawable->color.b;
    a = drawable->color.a;
    
    batchdata->colorData[colorstart + 0 * 4 + 0] = r;
    batchdata->colorData[colorstart + 0 * 4 + 1] = g;
    batchdata->colorData[colorstart + 0 * 4 + 2] = b;
    batchdata->colorData[colorstart + 0 * 4 + 3] = a; 
    batchdata->colorData[colorstart + 1 * 4 + 0] = r;
    batchdata->colorData[colorstart + 1 * 4 + 1] = g;
    batchdata->colorData[colorstart + 1 * 4 + 2] = b;
    batchdata->colorData[colorstart + 1 * 4 + 3] = a;  
    batchdata->colorData[colorstart + 2 * 4 + 0] = r;
    batchdata->colorData[colorstart + 2 * 4 + 1] = g;
    batchdata->colorData[colorstart + 2 * 4 + 2] = b;
    batchdata->colorData[colorstart + 2 * 4 + 3] = a;    
    batchdata->colorData[colorstart + 3 * 4 + 0] = r;
    batchdata->colorData[colorstart + 3 * 4 + 1] = g;
    batchdata->colorData[colorstart + 3 * 4 + 2] = b;
    batchdata->colorData[colorstart + 3 * 4 + 3] = a;   
    
    
    // write texture data only if sprite exists
    if ((sprite = drawable->sprite) == NULL)
        return;
        
    tx1 = Rectangle_getX(&sprite->rect) * sprite->texture->px;
    ty1 = Rectangle_getY(&sprite->rect) * sprite->texture->py; 
    tx2 = tx1 + Rectangle_getW(&sprite->rect) * sprite->texture->px;
    ty2 = ty1 + Rectangle_getH(&sprite->rect) * sprite->texture->py;    
       
    batchdata->textureData[texturestart + 0 * 2 + 0] = tx1;
    batchdata->textureData[texturestart + 0 * 2 + 1] = ty1;
    batchdata->textureData[texturestart + 1 * 2 + 0] = tx1;
    batchdata->textureData[texturestart + 1 * 2 + 1] = ty2; 
    batchdata->textureData[texturestart + 2 * 2 + 0] = tx2;
    batchdata->textureData[texturestart + 2 * 2 + 1] = ty2; 
    batchdata->textureData[texturestart + 3 * 2 + 0] = tx2;
    batchdata->textureData[texturestart + 3 * 2 + 1] = ty1;            
}


Drawable* Drawable_newBox()
{
    Drawable* ret = Drawable_new(4);
    ret->_writeBatchDataFunc = writeBatchData;
    ret->_getDataSizeFunc = getDataSize;
    ret->vertices[0].d[0] = 0;
    ret->vertices[0].d[1] = 0;
    ret->vertices[1].d[0] = 0;
    ret->vertices[1].d[1] = 0;
    
    return ret;
}

Drawable* Drawable_newBoxFromRectangle(Rectangle rect)
{
    Drawable* ret = Drawable_new(4);
    ret->_writeBatchDataFunc = writeBatchData;
    ret->_getDataSizeFunc = getDataSize;
    ret->vertices[0].d[0] = Rectangle_getX(&rect);
    ret->vertices[0].d[1] = Rectangle_getY(&rect);
    ret->vertices[1].d[0] = Rectangle_getX(&rect) + Rectangle_getW(&rect);
    ret->vertices[1].d[1] = Rectangle_getY(&rect) + Rectangle_getH(&rect);
    
    return ret;
}
