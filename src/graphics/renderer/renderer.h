#ifndef SMUG_GRAPHICS_RENDERER_RENDERER_H
#define SMUG_GRAPHICS_RENDERER_RENDERER_H

#include <utils/arraylist.h>
#include <graphics/drawable/drawable.h>
#include <graphics/camera.h>

// A renderer renders one layer of graphics objects from an internal list of drawables.
// A renderer should be able to take z-order into consideration if configured as such.

typedef struct Renderer
{
    ArrayList* layers;
    unsigned int layercount;
    Camera* camera;
} Renderer;

Renderer* Renderer_new(void);
void Renderer_delete(void* renderer);

void Renderer_render(Renderer* renderer);

void Renderer_addDrawable(Renderer* renderer, Drawable* drawable);

Camera* Renderer_getCamera(Renderer* renderer);

void Renderer_setupLayer(Renderer* renderer, unsigned int id, float parallax);

#endif // SMUG_GRAPHICS_RENDERER_RENDERER_H
