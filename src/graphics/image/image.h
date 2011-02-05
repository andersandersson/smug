/**
 * @file image.h
 * @brief Defines the Image type and related functions.
 * @ingroup smug_graphics
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_IMAGE_IMAGE_H
#define SMUG_GRAPHICS_IMAGE_IMAGE_H

#include <common/common.h>

/**
 * Holds image data, file name and meta data of an image.
 */
typedef struct Image
{
    unsigned char* data;
    unsigned char* file;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    int channels;
} Image;

/**
 * Create a new empty image to pass to the load function.
 * @relates Image
 */
Image* Image_new(void);

/**
 * Create an image based on raw image data.
 * @param data The image data.
 * @param size Number of bytes needed to store the image data in memory.
 * @param width Width of the image in pixels.
 * @param height Height of the image in pixels.
 * @param channels Number of bytes per pixel.
 * @relates Image
 */
Image* Image_newFromData(unsigned char* data, unsigned int size, unsigned int width, unsigned int height, int channels);

/**
 * Delete an image and its associated image data.
 * @param self The image to delete.
 * @relates Image
 */
void Image_delete(Image* self);

/**
 * Load an image from the specified file.
 *
 * The filename is interpreted in a platform-specific manner. Only PNG files are supported for now.
 *
 * @param filename The name of the image file to load.
 * @return TRUE on success, FALSE on failure.
 * @relates Image
 */
BOOL Image_loadFromFile(Image* self, const char* filename);

/**
 * Writes an image to a file.
 *
 * The filename is interpreted in a platform-specific manner. The file will be PNG-encoded.
 *
 * @param self The image to save.
 * @param filename The name of the file to save the image to.
 * @return TRUE on success, FALSE on failure.
 * @relates Image
 */
BOOL Image_saveToFile(Image* self, const char* filename);

#endif // SMUG_GRAPHICS_IMAGE_IMAGE_H

/**@}*/
