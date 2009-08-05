/** @file drawable.h
  * @brief Defines the Drawable type.
  */

#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include "sprite.h"
#include "graphics/color.h"
#include "utils/rectangle.h"

/** Something that can be rendered to the screen.
  *
  * @todo Rethink the rectangle member. A Rectangle has a position, but not a Sprite. We only need the width/height of the rectangle.
  */
typedef struct Drawable
{
    Sprite* sprite;
    Rectangle* rect;
    Color* color;

} Drawable;

/** Creates a Drawable from a Sprite.
  *
  * Calling Drawable_draw on the returned object will render the sprite.
  * 
  * @param s A pointer to a valid Sprite object.
  * @return A pointer to a new Drawable.
  */
Drawable* Drawable_newSprite(Sprite* s);

/** Creates a Drawable from a Rectangle and a color.
  *
  * Calling Drawable_draw on the returned object will render a filled rectangle of the specified color.
  *
  * @return A pointer to a new Drawable.
  */
Drawable* Drawable_newRect(Rectangle* r, Color* c);

void Drawable_delete(void* d);

/** Renders the Drawable.
  *
  * @todo This may need some coordinates too.
  *
  * @param d A pointer to a Drawable.
  */
void Drawable_render(Drawable* d, float x, float y);

#endif //SMUG_GRAPHICS_DRAWABLE_H
