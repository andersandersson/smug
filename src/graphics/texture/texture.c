#include <stdlib.h>
#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <platform/opengl/opengl.h>
#include <graphics/internal.h>

#include <graphics/texture/texture.h>
#include <graphics/texture/texture_type.h>
#include <graphics/texture/texture_internal.h>

static BOOL _invariant(Texture* self)
{
    smug_assert(self != NULL);
    return self != NULL;
}

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
        smug_assert(_invariant(tex));
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
        smug_assert(_invariant(tex));
        return tex;
    }
    free(tex);
    return NULL;
}

void Texture_release(Texture* self)
{
    smug_assert(_invariant(self));
	glDeleteTextures(1, &self->texid);
    printGLError();
	self->loaded = FALSE;
}

void Texture_reload(Texture* self)
{
    smug_assert(_invariant(self));
    if (FALSE != self->loaded)
    {
        Texture_release(self);
    }

    if (self->image != NULL)
    {
       loadTextureFromImage(self, self->image);
    }
    else
    {
        loadEmptyTexture(self, self->width, self->height);
    }
}

void Texture_delete(void* self)
{
    if (NULL != self)
    {
        smug_assert(_invariant(self));
        Texture* t = (Texture*)self;
        Texture_release(t);
        free(t);
    }
}

unsigned int Texture_getId(Texture* self)
{
    smug_assert(_invariant(self));
    return self->texid;
}

unsigned int Texture_getWidth(Texture* self)
{
    smug_assert(_invariant(self));
    return self->width;
}

unsigned int Texture_getHeight(Texture* self)
{
    smug_assert(_invariant(self));
    return self->height;
}
