#ifndef SMUG_GRAPHICS_RENDERER_RENDERER_H
#define SMUG_GRAPHICS_RENDERER_RENDERER_H

struct ArrayList;
struct GameObject;
struct Camera;
struct Drawable;

// A renderer renders one layer of graphics objects from an internal list of drawables.
// A renderer should be able to take z-order into consideration if configured as such.

typedef struct Renderer
{
    struct ArrayList* layers;
    unsigned int layercount;
    struct Camera* camera;
} Renderer;

Renderer* Renderer_new(void);
void Renderer_delete(void* renderer);

void Renderer_render(Renderer* renderer);

void Renderer_addDrawable(Renderer* renderer, struct Drawable* drawable);

struct Camera* Renderer_getCamera(Renderer* renderer);

void Renderer_setupLayer(Renderer* renderer, unsigned int id, float parallax);

#endif // SMUG_GRAPHICS_RENDERER_RENDERER_H
