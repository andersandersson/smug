#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H

#include <common/common.h>
#include <graphics/color_type.h>

struct GameObject;
struct Sprite;
struct BatchData;
struct Texture;


struct GameObject* Drawable_new();

BOOL Drawable_getColor(struct GameObject* self, Color* c);
BOOL Drawable_setColor(struct GameObject* self, Color c);
BOOL Drawable_getOpacity(struct GameObject* self, float* opacity);
BOOL Drawable_setOpacity(struct GameObject* self, float opacity);
struct Sprite* Drawable_getSprite(struct GameObject* self);
unsigned int Drawable_getLayer(struct GameObject* self);
void Drawable_setLayer(struct GameObject* self, unsigned int layer);
void Drawable_writeBatchData(struct GameObject* self, struct BatchData* batchdata, unsigned int start);
int Drawable_getDataSize(struct GameObject* self);
unsigned int Drawable_getObjectSize(struct GameObject* self);

struct Texture* Drawable_getTexture(struct GameObject* d);
unsigned int Drawable_getTextureID(struct GameObject* d);

struct GameObject* Drawable_newInherit(
    void(*writeBatchDataFunc)(struct GameObject* d, struct BatchData* batch, unsigned int start),
    int (*getDataSizeFunc)(struct GameObject* d),
    unsigned int (*getObjectSizeFunc)(struct GameObject* d));

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLE_H
