#include <stdlib.h>
#include <smugstd.h>

#include <common/common.h>
#include <utils/point.h>
#include <utils/rectangle.h>
#include <utils/vector.h>
#include <utils/shapes.h>
#include <engine/gameobject.h>
#include <engine/gameobject_protected.h>
#include <engine/position_object.h>
#include <graphics/color.h>
#include <graphics/sprite.h>
#include <graphics/renderer/batchdata.h>
#include <graphics/drawable/drawable.h>
#include <graphics/drawable/drawableshape.h>
#include <graphics/drawable/drawableshape_type.h>

#include <graphics/drawable/box.h>

static BOOL _invariant(DrawableShapeData* data)
{
    smug_assert(data != NULL);
    smug_assert(data->mShape != NULL);
    smug_assert(data->mShape->type == SHAPE_RECTANGLE);
    return (data != NULL && data->mShape != NULL && data->mShape->type == SHAPE_RECTANGLE);
}

static int getDataSize(GameObject* self)
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

static void writeBatchData(GameObject* drawable, BatchData* batchdata, unsigned int start)
{
	smug_assert(GameObject_isType(drawable, SMUG_TYPE_DRAWABLE));
    static unsigned int vertexstart, colorstart, texturestart;
    static float x1, x2, y1, y2;
    static float r, g, b, a;
    static float tx1, ty1, tx2, ty2;
    static Sprite* sprite = NULL;
    static Point dpos;
    static Shape* shape;
    static Rectangle box;
    static Color color;

    vertexstart = start*2;
    colorstart = start*4;
    texturestart = start*2;

    DrawableShape_getShape(drawable, &shape);
    box = Shape_getAsRectangle(shape);

    // write vertices in anti-clockwise order
    PositionObject_getPosForDrawing(drawable, &dpos);
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

    // write colordata
    Drawable_getColor(drawable, &color);
    r = Color_Rf(color);
    g = Color_Gf(color);
    b = Color_Bf(color);
    a = Color_Af(color);

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

static DrawableShapeData* DrawableShapeData_newBox(Vector size)
{
    DrawableShapeData* data = (DrawableShapeData*)malloc(sizeof(DrawableShapeData));
    struct Shape* newShape = Shape_newFromRectangle(Rectangle_createFromXYWH(0, 0, Vector_getX(&size), Vector_getY(&size)));
    data->mShape = newShape;
    smug_assert(_invariant(data));
    return data;
}

GameObject* DrawableShape_newBox(void)
{
    return DrawableShape_newBoxFromSize(Vector_create2d(0,0));
}

GameObject* DrawableShape_newBoxFromSize(Vector size)
{
    InternalGameObject* leaf = DrawableShape_newInherit(DrawableShapeData_newBox(size), writeBatchData, getDataSize);
    smug_assert(InternalGameObject_isExactType(leaf, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(leaf, SMUG_TYPE_SHAPE));
    smug_assert(GameObject_isType(leaf, SMUG_TYPE_DRAWABLE));
    smug_assert(GameObject_isType(leaf, SMUG_TYPE_POSITION));
    smug_assert(GameObject_isType(leaf, SMUG_TYPE_OBJECT));
    return InternalGameObject_getAsGeneric(leaf);
}
