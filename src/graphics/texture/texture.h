#ifndef SMUG_GRAPHICS_TEXTURE_TEXTURE_H
#define SMUG_GRAPHICS_TEXTURE_TEXTURE_H

#include <common/common.h>
#include <graphics/image/image.h>
#include <graphics/texture/texture_type.h>

Texture* Texture_new(unsigned int width, unsigned int height);
Texture* Texture_newFromImage(Image* image);

void Texture_release(Texture* texture);
void Texture_reload(Texture* texture);
void Texture_delete(void* texture);

unsigned int Texture_getId(Texture* self);
unsigned int Texture_getWidth(Texture* self);
unsigned int Texture_getHeight(Texture* self);

#endif // SMUG_GRAPHICS_TEXTURE_TEXTURE_H
