#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H

#include <common/common.h>
#include <graphics/color_type.h>
#include <graphics/texture/texture_type.h>
#include <graphics/drawable/drawable_type.h>

struct Sprite;
struct BatchData;

Drawable* Drawable_new(void);

BOOL Drawable_getColor(Drawable* self, Color* c);
BOOL Drawable_setColor(Drawable* self, Color c);
BOOL Drawable_getUseColor(Drawable* self, BOOL* use);
BOOL Drawable_setUseColor(Drawable* self, BOOL use);
BOOL Drawable_getOpacity(Drawable* self, float* opacity);
BOOL Drawable_setOpacity(Drawable* self, float opacity);
struct Sprite* Drawable_getSprite(Drawable* self);
void Drawable_setSprite(Drawable* self, struct Sprite* sprite);
unsigned int Drawable_getLayer(Drawable* self);
void Drawable_setLayer(Drawable* self, unsigned int layer);
void Drawable_writeBatchData(Drawable* self, struct BatchData* batchdata, unsigned int start);
int Drawable_getDataSize(Drawable* self);
unsigned int Drawable_getObjectSize(Drawable* self);

Texture* Drawable_getTexture(Drawable* d);
unsigned int Drawable_getTextureID(Drawable* d);

void Drawable_deInit(Drawable* self);
void Drawable_init(Drawable* self);

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
