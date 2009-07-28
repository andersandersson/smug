#ifndef SMUG_GRAPHICS_IMAGE_H
#define SMUG_GRAPHICS_IMAGE_H

#include "../common/common.h"

typedef struct Image
{
    unsigned char* data;
    unsigned char* file;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    int bpp;
    int channels;
} Image;

Image* Image_new();
void Image_free(Image* image);

BOOL Image_loadFromFile(Image* image, const char* filename);
BOOL Image_saveToFile(Image* image, const char* filename);


#endif
