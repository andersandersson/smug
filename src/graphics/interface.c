#include <common/common.h>
#include <utils/rectangle.h>
#include <graphics/color.h>
#include <graphics/drawable/box.h>
#include <graphics/drawable/drawable.h>

#include <graphics/interface.h>

SmugDrawable smugDrawable_newFromRect(SmugRect rect)
{
    return Drawable_newBoxFromSize(Rectangle_getSize(rect));
}

void smugDrawable_setColor(SmugDrawable drawable, SmugColor color)
{
    Drawable_setColor(drawable, color);
}

SmugColor smugColor_create(int r, int g, int b, int a)
{
    return Color_createFromRGBAi(r, g, b, a);
}
