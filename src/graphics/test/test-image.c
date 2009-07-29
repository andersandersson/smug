#include <stdio.h>

#include "image/image.h"

#include "log.h"

unsigned char reference[4][4][4] = {{{0, 0, 0, 255 }, {255, 255, 255, 0 }, {255, 255, 255, 0 }, {255, 0, 0, 255 }}, 
                          {{255, 255, 255, 0 }, {255, 255, 255, 255 }, {255, 255, 255, 255 }, {255, 255, 255, 0 }},
                          {{255, 255, 255, 0 }, {255, 255, 255, 255 }, {255, 255, 255, 255 }, {255, 255, 255, 0 }},
                          {{0, 255, 0, 255 }, {255, 255, 255, 0 }, {255, 255, 255, 0 }, {0, 0, 255, 255 }}};

int main()
{
    Log_init();
    Image* image = Image_new();
    
    if (!Image_loadFromFile(image, "test-image.png"))
    {
        fprintf(stderr, "Loading image failed.\n");
        Image_free(image);
        return 1;
    }
    
    if (image->width * image->height * image->channels != image->size)
    {
        fprintf(stderr, "Image size incorrect.\n");
        fprintf(stderr, "Image width: %i, height: %i, channels: %i, size %i \n", image->width, image->height, image->channels, image->size);
        return 1;
    }
    
    int i;
    int j;
    int k;
    for(i = 0; i < image->height; i++)
    {
        for(j = 0; j < image->width; j++)
        {
            for(k = 0; k < image->channels; k++)
            {
                if (image->data[(i * image->channels * image->width) + (j * image->channels) + k] != reference[i][j][k])
                {
                    fprintf(stderr, "Image loaded is not the same as refrence at (%i, %i, %i)\n", i, j, k);
                    return 1;
                }
            }
        }
    }
    
    Image_free(image);

    Log_terminate();

    return 0;
}
