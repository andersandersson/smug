#include "sprite.h"
#include <stdlib.h>

Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offset)
{
    assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = rect;
    s->offset = offset;
    s->base.layer = 0;
    s->base._deleteFunc = Sprite_delete;
    s->base._writeBatchFunc = Sprite_writeBatchData;
    
    return s;
}

Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset)
{
    assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = Rectangle_multiplyVector(rect, Vector_create2d(texture->width, texture->height));
    s->offset = Vector_multiplyVector(offset, Vector_create2d(texture->width, texture->height));
    s->base.layer = 0;
    s->base._deleteFunc = Sprite_delete;
    s->base._writeBatchFunc = Sprite_writeBatchData;
    
    return s;
}

void Sprite_delete(void* sprite)
{
    if (NULL != sprite)
    {
        free(sprite);
    }
}


int Sprite_writeBatchData(Drawable* sprite, RenderBatch* batch, unsigned int start)
{
    static unsigned int vertexstart, colorstart, texturestart;
    static float x1, x2, y1, y2;
    static float r, g, b, a;
    static float tx1, ty1, tx2, ty2;
    
    vertexstart = start*2;
    colorstart = start*4; 
    texturestart = start*2; 
    Sprite* thesprite = (Sprite*)sprite;
    
    x1 = Point_getX(&thesprite->base.pos) - thesprite->offset.d[0];
    y1 = Point_getY(&thesprite->base.pos) - thesprite->offset.d[1]; 
    x2 = x1 + Rectangle_getW(&thesprite->rect);
    y2 = y1 + Rectangle_getH(&thesprite->rect);
   
    // write vertices in anti-clockwise order
    batch->vertexData[vertexstart + 0 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 0 * 2 + 1] = y1;
    batch->vertexData[vertexstart + 1 * 2 + 0] = x1;
    batch->vertexData[vertexstart + 1 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 2 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 2 * 2 + 1] = y2; 
    batch->vertexData[vertexstart + 3 * 2 + 0] = x2;
    batch->vertexData[vertexstart + 3 * 2 + 1] = y1;   
    
    r = thesprite->color.r;
    g = thesprite->color.g;
    b = thesprite->color.b;
    a = thesprite->color.a;
    
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
       
    tx1 = Rectangle_getX(&thesprite->rect) * thesprite->texture->px;
    ty1 = Rectangle_getY(&thesprite->rect) * thesprite->texture->py; 
    tx2 = tx1 + Rectangle_getW(&thesprite->rect) * thesprite->texture->px;
    ty2 = ty1 + Rectangle_getH(&thesprite->rect) * thesprite->texture->py;    
       
    // write texture data
    batch->textureData[texturestart + 0 * 2 + 0] = tx1;
    batch->textureData[texturestart + 0 * 2 + 1] = ty1;
    batch->textureData[texturestart + 1 * 2 + 0] = tx1;
    batch->textureData[texturestart + 1 * 2 + 1] = ty2; 
    batch->textureData[texturestart + 2 * 2 + 0] = tx2;
    batch->textureData[texturestart + 2 * 2 + 1] = ty2; 
    batch->textureData[texturestart + 3 * 2 + 0] = tx2;
    batch->textureData[texturestart + 3 * 2 + 1] = ty1;      
    
      
    // number of vertices written
    return 4;
}
