#include <stdlib.h>
#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <platform/opengl/opengl.h>
#include <graphics/internal.h>

#include <graphics/texture/texture.h>
#include <graphics/texture/texture_type.h>
#include <graphics/texture/texture_internal.h>

static int getClosestGreaterPowerOfTwo(int number)
{
    int n = 0;
    double power = ldexp(1.0, n);
    while (power < number)
    {
        ++n;
        power = ldexp(1.0, n);
    }
    return floor(power + 0.5);
}

static Texture* loadTextureFromImage(Texture* tex, Image* image)
{
    smug_assert(image != NULL);

    unsigned int texid;

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getClosestGreaterPowerOfTwo(image->width), getClosestGreaterPowerOfTwo(image->height), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
#ifdef SMUG_GLES
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
#ifndef SMUG_GLES
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

#ifndef SMUG_GLES /* OpenGL ES 1.0 does not support glIsTexture */
    if (glIsTexture(texid) != GL_TRUE)
    {
        ERROR("Could not load texture from image.");
        return NULL;
    }
#endif

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
    printGLError();

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
    printGLError();
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

unsigned int Texture_getId(Texture* self)
{
    return self->texid;
}
