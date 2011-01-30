#include <stdlib.h>
#include <smugstd.h>

#include <common/common.h>
#include <utils/point.h>
#include <utils/rectangle.h>
#include <utils/vector.h>
#include <utils/shape.h>
#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/positionedobject.h>
#include <graphics/color.h>
#include <graphics/sprite.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/texture_internal.h>
#include <graphics/renderer/batchdata.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawable_internal.h>

#include <graphics/drawable/box.h>

static BOOL _invariant(Drawable* self)
{
    smug_assert(self != NULL);
    smug_assert(self->mShape != NULL);
    smug_assert(Shape_getType(self->mShape) == SHAPE_RECTANGLE);
    return (self != NULL && self->mShape != NULL && Shape_getType(self->mShape) == SHAPE_RECTANGLE);
}

static int getDataSize(Drawable* self)
{
    // TODO: smug_assert("That 'self' is actually a box, or at least a DrawableShape");

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
    smug_assert(_invariant(drawable));
    smug_assert(GameObject_isType((GameObject*)drawable, SMUG_TYPE_DRAWABLE));
    static unsigned int vertexstart, colorstart, texturestart;
    static float x1, x2, y1, y2;
    static float r, g, b, a;
    static float tx1, ty1, tx2, ty2;
    static Sprite* sprite = NULL;
    static Point dpos;
    static Shape* shape;
    static Rectangle box;
    static Color color;
    static BOOL useColor;

    vertexstart = start*2;
    colorstart = start*4;
    texturestart = start*2;

    Drawable_getShape(drawable, &shape);
    box = Shape_getAsRectangle(shape);

    // write vertices in anti-clockwise order
    PositionedObject_getPosForDrawing((GameObject*)drawable, &dpos);
    x1 = Point_getX(dpos) + Rectangle_getX(&box);
    y1 = Point_getY(dpos) + Rectangle_getY(&box);
    x2 = Point_getX(dpos) + Rectangle_getW(&box);
    y2 = Point_getY(dpos) + Rectangle_getH(&box);

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


    Drawable_getUseColor(drawable, &useColor);
    Drawable_getColor(drawable, &color);
    a = Color_Af(color);
    if (useColor)
    {
        // write colordata
        r = Color_Rf(color);
        g = Color_Gf(color);
        b = Color_Bf(color);
    }
    else
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }

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
    if ((sprite = Drawable_getSprite(drawable)) == NULL)
    {
        return;
    }

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
    return Drawable_newBoxFromSize(Vector_create2d(0,0));
}

Drawable* Drawable_newBoxFromSize(Vector size)
{
    Drawable* newObj = (Drawable*)malloc(sizeof(_Drawable));
    Drawable_init(newObj);
    newObj->_writeBatchDataFunc = writeBatchData;
    newObj->_getDataSizeFunc = getDataSize;
    newObj->mShape = Shape_newFromRectangle(Rectangle_createFromXYWH(0, 0, Vector_getX(&size), Vector_getY(&size)));
    smug_assert(_invariant(newObj));
    return newObj;
/*    smug_assert(GameObject_isExactType(newObj, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(newObj, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(newObj, SMUG_TYPE_DRAWABLE));
    smug_assert(GameObject_isType(newObj, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(newObj, SMUG_TYPE_OBJECT)); */
}

Drawable* Drawable_newFromSprite(Sprite* sprite)
{
    Drawable* newObj = Drawable_newBoxFromSize(Vector_create2d(Texture_getWidth(Sprite_getTexture(sprite)), Texture_getHeight(Sprite_getTexture(sprite))));
    Drawable_setSprite(newObj, sprite);
    Drawable_setUseColor(newObj, FALSE);
    return newObj;
}
