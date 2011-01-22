#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H

#include <common/common.h>
#include <graphics/color_type.h>

struct Sprite;
struct BatchData;
struct Texture;
struct Drawable;

struct Drawable* Drawable_new(void);

BOOL Drawable_getColor(struct Drawable* self, Color* c);
BOOL Drawable_setColor(struct Drawable* self, Color c);
BOOL Drawable_getOpacity(struct Drawable* self, float* opacity);
BOOL Drawable_setOpacity(struct Drawable* self, float opacity);
struct Sprite* Drawable_getSprite(struct Drawable* self);
unsigned int Drawable_getLayer(struct Drawable* self);
void Drawable_setLayer(struct Drawable* self, unsigned int layer);
void Drawable_writeBatchData(struct Drawable* self, struct BatchData* batchdata, unsigned int start);
int Drawable_getDataSize(struct Drawable* self);
unsigned int Drawable_getObjectSize(struct Drawable* self);

struct Texture* Drawable_getTexture(struct Drawable* d);
unsigned int Drawable_getTextureID(struct Drawable* d);

void Drawable_deInit(struct Drawable* self);
void Drawable_init(struct Drawable* self);

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
