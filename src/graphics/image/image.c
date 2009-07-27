#include "image.h"

#include "stdlib.h"

#include "lodepng/lodepng.h"
#include "common/log.h"
#include "common/common.h"

Image* Image_new()
{
    Image* ret = (Image*)malloc(sizeof(Image));
    ret->data = NULL;
    ret->file = NULL; 
    ret->size = 0;
    ret->width = 0;
    ret->height = 0;
    ret->bpp = 0;
    ret->channels = 0;
    
    return ret;
}
void Image_free(Image* image)
{
    if (image->data)
    {
        free(image->data);
        image->data = NULL;
    }
    if (image->file)
    {
        free(image->file);
        image->file = NULL;
    } 
   
    free(image);
}

unsigned char* _loadFile(const char* filename, unsigned int* buffersize)
{
 	FILE* file = fopen(filename,"r+b");
	if (!file)
	{
		WARNING("Couldn't locate file '%s'.", filename);
		return 0;
	}   
    DEBUG("Successfully opened file '%s'.", filename);
    
	fseek(file, 0, SEEK_END);
	int filelen = ftell(file);
	fseek(file, 0, SEEK_SET);
    
    DEBUG("File length: '%i'.", filelen);   
    
	unsigned char* buffer = (unsigned char*)malloc(filelen);
	if (fread(buffer, 1, filelen, file) != filelen)
	{
		ERROR("Did not read correct amount of bytes.");
		fclose(file);
		free(buffer);
        return 0;
	}    
    
    fclose(file);
    *buffersize = filelen;
    return buffer;
}

BOOL _decodePNG(Image* image, unsigned char* buffer, unsigned int buffersize)
{     
    LodePNG_Decoder decoder;
    LodePNG_Decoder_init(&decoder);
    LodePNG_decode(&decoder, &image->data, &image->size, buffer, buffersize);
      
    if(decoder.error) 
    {   
        ERROR("PNG decoding failed, error: %d\n", decoder.error);
        return FALSE;
    }

    image->width = decoder.infoPng.width;
    image->height = decoder.infoPng.height;
    image->bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
    image->channels = LodePNG_InfoColor_getChannels(&decoder.infoPng.color);

    LodePNG_Decoder_cleanup(&decoder);

    return TRUE;
}


BOOL Image_loadFromFile(Image* image, const char* filename)
{
    unsigned char* buffer = NULL;  
    
    unsigned int buffersize;
    buffer = _loadFile(filename, &buffersize);
    if (NULL == buffer)
    {
        WARNING("Could not open image '%s'", filename);
        return FALSE;
    }
    
    //if last three letters are png
    int len = strlen(filename);
    BOOL retval = FALSE;
    if (0 == strncmp(&filename[len-4], "png", 3))
    {
        DEBUG("Decoding image as PNG");
        retval = _decodePNG(image, buffer, buffersize);
    }
    else
    {
        DEBUG("No known file format, decoding image as PNG");
        retval = _decodePNG(image, buffer, buffersize);
    }
    
    if (!retval)
    {
        ERROR("Could not load image");
        return FALSE;
    }

    DEBUG("Successfully loaded image");
    return TRUE;
}

BOOL Image_saveToFile(Image* image, const char* filename)
{
    unsigned char* buffer = NULL;    

    
    //TODO: add implementation
    WARNING("Save image not yet implemented");

}
