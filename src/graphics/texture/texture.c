#include <smugstd.h>
#include "texture.h"

#include <stdlib.h>
#include "platform/opengl/opengl.h"
#include "common/log.h"

static Texture* loadTextureFromImage(Texture* tex, Image* image)
{
    smug_assert(image != NULL);

    unsigned int texid;

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (glIsTexture(texid) != GL_TRUE)
    {
        ERROR("Could not load texture from image.");
        return NULL;
    }

    tex->texid = texid;
    tex->image = image;
    tex->width = image->width;
    tex->height = image->height;
    tex->px = 1.0/tex->width;
    tex->py = 1.0/tex->height;
    tex->loaded = TRUE;

    return tex;
}

static BOOL loadEmptyTexture(Texture* tex, unsigned int width, unsigned height)
{
    unsigned int texid;

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexImage2D(GL_TEXTURE_2D, GL_RGBA, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (glIsTexture(texid) != GL_TRUE)
    {
        ERROR("Could not load empty texture.");
        return FALSE;
    }

    tex->texid = texid;
    tex->image = NULL;
    tex->width = width;
    tex->height = height;
    tex->px = 1.0/tex->width;
    tex->py = 1.0/tex->height;
    tex->loaded = TRUE;

    return TRUE;
}

Texture* Texture_new(unsigned int width, unsigned int height)
{
    Texture* tex = (Texture*)malloc(sizeof(Texture));
    if (loadEmptyTexture(tex, width, height))
    {
        return tex;
    }
    free(tex);
    return NULL;
}

Texture* Texture_newFromImage(Image* image)
{
    Texture* tex = (Texture*)malloc(sizeof(Texture));
    if (loadTextureFromImage(tex, image))
    {
        return tex;
    }
    free(tex);
    return NULL;
}

void Texture_release(Texture* texture)
{
	glDeleteTextures(1, &texture->texid);
	texture->loaded = FALSE;
}

void Texture_reload(Texture* texture)
{
    if (FALSE != texture->loaded)
    {
        Texture_release(texture);
    }

    if (texture->image != NULL)
    {
       loadTextureFromImage(texture, texture->image);
    }
    else
    {
        loadEmptyTexture(texture, texture->width, texture->height);
    }
}

void Texture_delete(void* texture)
{
    if (NULL != texture)
    {
        Texture* t = (Texture*)texture;
        Texture_release(t);
        free(t);
    }
}
