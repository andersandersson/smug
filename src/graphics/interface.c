#include <common/common.h>
#include <common/error.h>
#include <engine/gameobject.h>
#include <utils/rectangle.h>
#include <graphics/color.h>
#include <graphics/drawable/box.h>
#include <graphics/drawable/drawable.h>

#include <graphics/interface.h>

SmugObject smugDrawable_newFromRect(SmugRect rect)
{
    return DrawableShape_newBoxFromSize(Rectangle_getSize(rect));
}

void smugDrawable_setColor(SmugObject drawable, SmugColor color)
{
    smug_error(GameObject_isType(drawable, SMUG_TYPE_DRAWABLE), "Wrong type for smugDrawable_setColor!");
    Drawable_setColor(drawable, color);
}

SmugColor smugColor_create(int r, int g, int b, int a)
{
    return Color_createFromRGBAi(r, g, b, a);
}
