#ifndef SMUG_GRAPHICS_IMAGE_H
#define SMUG_GRAPHICS_IMAGE_H

#include "common/common.h"

typedef struct Image
{
    unsigned char* data;
    unsigned char* file;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    int channels;
} Image;

Image* Image_new();
Image* Image_newFromData(unsigned char* data, unsigned int size, unsigned int width, unsigned int height, int channels);
void Image_delete(Image* image);

BOOL Image_loadFromFile(Image* image, const char* filename);
BOOL Image_saveToFile(Image* image, const char* filename);

#endif //SMUG_GRAPHICS_IMAGE_H
