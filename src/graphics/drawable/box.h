#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H


#include "drawable.h"
#include "graphics/renderer/renderbatch.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

Drawable* Drawable_newBox();
Drawable* Drawable_newBoxFromRectangle(Rectangle rect);

#endif //GRAPHICS_DRAWABLE_BOX_H
