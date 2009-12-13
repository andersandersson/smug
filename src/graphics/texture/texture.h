#ifndef SMUG_GRAPHICS_TEXTURE_H
#define SMUG_GRAPHICS_TEXTURE_H

#include "common/common.h"
#include "graphics/image/image.h"

typedef struct Texture
{
    Image* image;
    unsigned int texid;
    unsigned int width;
    unsigned int height;
    float px;
    float py;
    BOOL loaded;
} Texture;

Texture* Texture_new(unsigned int width, unsigned int height);
Texture* Texture_newFromImage(Image* image);

void Texture_release(Texture* texture);
void Texture_reload(Texture* texture);
void Texture_delete(void* texture);

#endif //SMUG_GRAPHICS_TEXTURE_H
