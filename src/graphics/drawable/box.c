#include "box.h"
#include <stdlib.h>
#include <smugstd.h>

static int getDataSize(Drawable* drawable)
{
    // number of vertices that shall be written
    // OpenGL ES does not support GL_QUADS
#ifdef SMUG_GLES
    return 6;
#else
    return 4;
#endif /* SMUG_GLES */
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

    // smug_printf("Writing box: %f, %f, %f, %f", x1, y1, x2, y2);

    batchdata->vertexData[vertexstart + 0 * 2 + 0] = x1;
    batchdata->vertexData[vertexstart + 0 * 2 + 1] = y1;
    batchdata->vertexData[vertexstart + 1 * 2 + 0] = x1;
    batchdata->vertexData[vertexstart + 1 * 2 + 1] = y2;
    batchdata->vertexData[vertexstart + 2 * 2 + 0] = x2;
    batchdata->vertexData[vertexstart + 2 * 2 + 1] = y2;
    batchdata->vertexData[vertexstart + 3 * 2 + 0] = x2;
    batchdata->vertexData[vertexstart + 3 * 2 + 1] = y1;
#ifdef SMUG_GLES
    batchdata->vertexData[vertexstart + 4 * 2 + 0] = x1;
    batchdata->vertexData[vertexstart + 4 * 2 + 1] = y1;
    batchdata->vertexData[vertexstart + 5 * 2 + 0] = x2;
    batchdata->vertexData[vertexstart + 5 * 2 + 1] = y2;
#endif /* SMUG_GLES */

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
#ifdef SMUG_GLES
    batchdata->colorData[colorstart + 4 * 4 + 0] = r;
    batchdata->colorData[colorstart + 4 * 4 + 1] = g;
    batchdata->colorData[colorstart + 4 * 4 + 2] = b;
    batchdata->colorData[colorstart + 4 * 4 + 3] = a;
    batchdata->colorData[colorstart + 5 * 4 + 0] = r;
    batchdata->colorData[colorstart + 5 * 4 + 1] = g;
    batchdata->colorData[colorstart + 5 * 4 + 2] = b;
    batchdata->colorData[colorstart + 5 * 4 + 3] = a;
#endif /* SMUG_GLES */

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
#ifdef SMUG_GLES
    batchdata->textureData[texturestart + 4 * 2 + 0] = tx1;
    batchdata->textureData[texturestart + 4 * 2 + 1] = ty1;
    batchdata->textureData[texturestart + 5 * 2 + 0] = tx2;
    batchdata->textureData[texturestart + 5 * 2 + 1] = ty2;
#endif /* SMUG_GLES */
}

Drawable* Drawable_newBox(void)
{
#ifdef SMUG_GLES
    Drawable* ret = Drawable_new(6);
#else
    Drawable* ret = Drawable_new(4);
#endif
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
#ifdef SMUG_GLES
    Drawable* ret = Drawable_new(6);
#else
    Drawable* ret = Drawable_new(4);
#endif
    ret->_writeBatchDataFunc = writeBatchData;
    ret->_getDataSizeFunc = getDataSize;
    ret->vertices[0].d[0] = Rectangle_getX(&rect);
    ret->vertices[0].d[1] = Rectangle_getY(&rect);
    ret->vertices[1].d[0] = Rectangle_getX(&rect) + Rectangle_getW(&rect);
    ret->vertices[1].d[1] = Rectangle_getY(&rect) + Rectangle_getH(&rect);

    return ret;
}
