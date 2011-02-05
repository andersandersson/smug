#include <stdlib.h>
#include <string.h>

#include <common/common.h>
#include <common/log.h>
#include <platform/file.h>
#include <graphics/image/lodepng/lodepng.h>

#include <graphics/image/image.h>

Image* Image_new(void)
{
    Image* ret = (Image*)malloc(sizeof(Image));
    ret->data = NULL;
    ret->file = NULL;
    ret->size = 0;
    ret->width = 0;
    ret->height = 0;
    ret->channels = 0;

    return ret;
}

Image* Image_newFromData(unsigned char* data, unsigned int size, unsigned int width, unsigned int height, int channels)
{
    Image* ret = (Image*)malloc(sizeof(Image));
    ret->size = size;
    ret->channels = channels;
    ret->width = width;
    ret->height = height;
    if (ret->channels * ret->width * ret->height != ret->size)
    {
        WARNING("Size of data does not match dimensions of image.");
        DEBUG("Image width: %i, height: %i, channels: %i, size %i", ret->width, ret->height, ret->channels, ret->size);
    }
    ret->data = (unsigned char*)malloc(sizeof(unsigned char) * ret->size);
    memcpy(ret->data, data, ret->size);
    ret->file = NULL;
    return ret;
}

void Image_delete(Image* self)
{
    if (self->data)
    {
        free(self->data);
        self->data = NULL;
    }
    if (self->file)
    {
        free(self->file);
        self->file = NULL;
    }

    free(self);
}

static unsigned char* _loadFile(const char* filename, unsigned int* buffersize)
{
 	File* file = File_fopen(filename,"rb");
	if (!file)
	{
		WARNING("Couldn't locate file '%s'.", filename);
		return 0;
	}
    DEBUG("Successfully opened file '%s'.", filename);

	File_fseek(file, 0, SMUG_SEEK_END);
	int filelen = File_ftell(file);
	File_fseek(file, 0, SMUG_SEEK_SET);

    DEBUG("File length: '%i'.", filelen);

	unsigned char* buffer = (unsigned char*)malloc(filelen);
	if (File_fread(file, buffer, 1, filelen) != filelen)
	{
		ERROR("Did not read correct amount of bytes.");
		File_fclose(file);
		free(buffer);
        return 0;
	}

    File_fclose(file);
    *buffersize = filelen;
    return buffer;
}

static BOOL _saveFile(const char* filename, unsigned char* buffer, unsigned int buffersize)
{
 	FILE* file = fopen(filename,"w+b");
	if (!file)
	{
		WARNING("Couldn't locate or create file '%s'.", filename);
		return FALSE;
	}
    DEBUG("Successfully opened file '%s'.", filename);


    DEBUG("Buffer length: '%i'.", buffersize);

	if (fwrite(buffer, 1, buffersize, file) != buffersize)
	{
		ERROR("Did not write correct amount of bytes.");
		fclose(file);
        return FALSE;
	}

    fclose(file);

    return TRUE;
}

static BOOL _decodePNG(Image* image, unsigned char* buffer, unsigned int buffersize)
{
    LodePNG_Decoder decoder;
    LodePNG_Decoder_init(&decoder);
    LodePNG_decode(&decoder, &image->data, &image->size, buffer, buffersize);

    if(decoder.error)
    {
        ERROR("PNG decoding failed, error: %d", decoder.error);
        return FALSE;
    }

    image->width = decoder.infoPng.width;
    image->height = decoder.infoPng.height;
    image->channels = LodePNG_InfoColor_getChannels(&decoder.infoPng.color);

    LodePNG_Decoder_cleanup(&decoder);

    return TRUE;
}

static BOOL _encodePNG(unsigned char** buffer, unsigned int* buffersize, Image* image)
{
    LodePNG_Encoder encoder;
    LodePNG_Encoder_init(&encoder);
    LodePNG_encode(&encoder, buffer, buffersize, image->data, image->width, image->height);

    if(encoder.error)
    {
        ERROR("PNG encoding failed, error: %d", encoder.error);
        return FALSE;
    }

    LodePNG_Encoder_cleanup(&encoder);

    return TRUE;
}

BOOL Image_loadFromFile(Image* self, const char* filename)
{
    unsigned char* buffer = NULL;

    DEBUG("Loading image from file '%s'", filename);

    unsigned int buffersize;
    buffer = _loadFile(filename, &buffersize);
    if (NULL == buffer)
    {
        WARNING("Could not open image '%s'", filename);
        return FALSE;
    }

    // Decode according to file ending
    int len = strlen(filename);
    BOOL retval = FALSE;
    if (0 == strncmp(&filename[len-4], ".png", 3))
    {
        DEBUG("Decoding image as PNG");
        retval = _decodePNG(self, buffer, buffersize);
    }
    else
    {
        DEBUG("No known file format, decoding image as PNG");
        retval = _decodePNG(self, buffer, buffersize);
    }

    if (!retval)
    {
        ERROR("Could not load image");
        return FALSE;
    }

    DEBUG("Successfully loaded image");
    return TRUE;
}

BOOL Image_saveToFile(Image* self, const char* filename)
{
    unsigned char* buffer = NULL;
    unsigned int buffersize;
    DEBUG("Saving image to file '%s'", filename);

    // Encode according to file ending
    int len = strlen(filename);
    BOOL retval = FALSE;
    DEBUG("Comparing '%s' to known file endings", &filename[len-4]);
    if (0 == strncmp(&filename[len-4], ".png", 3))
    {
        DEBUG("Encoding image as PNG");
        retval = _encodePNG(&buffer, &buffersize, self);
    }
    else
    {
        DEBUG("No known file format, encoding image as PNG");
        retval = _encodePNG(&buffer, &buffersize, self);
    }

    if (!retval || NULL == buffer)
    {
        ERROR("Could not save image");
        return FALSE;
    }

    retval = _saveFile(filename, buffer, buffersize);

    return retval;
}
