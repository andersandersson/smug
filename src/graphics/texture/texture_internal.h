#ifndef SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H
#define SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H

typedef struct _Texture
{
    Image* image;
    unsigned int texid;
    unsigned int width;
    unsigned int height;
    float px;
    float py;
    BOOL loaded;
} _Texture;

#endif /* SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H */
