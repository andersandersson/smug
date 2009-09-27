#include "renderer.h"

#include "renderbatch.h"
#include "layer.h"

#include "stdlib.h"
#include "stdio.h"

#include "graphics/sprite.h"
#include "graphics/graphics.h"

#include "platform/opengl/opengl.h"


extern int gVBOSupported;

extern int gScreenWidth;
extern int gScreenHeight;


Renderer* Renderer_new()
{
    Renderer* ret = (Renderer*)malloc(sizeof(Renderer));
    ret->layers = ArrayList_new();
    ret->layercount = 0;
    ret->camera = Camera_new(Vector_multiply(Graphics_getScreenSize(), 2));
    
    return ret;
}

void Renderer_delete(void* renderer)
{
    ArrayList_deleteContents(((Renderer*)renderer)->layers, Layer_delete);
    ArrayList_delete(((Renderer*)renderer)->layers);
}

static void renderLayer(Renderer* renderer, Layer* layer)
{
    Camera* camera = renderer->camera;
    float p = Layer_getParallax(layer);


    glPushMatrix();
    
    glTranslatef(camera->center.d[0], camera->center.d[1], 0.0f);
    glRotatef(renderer->camera->rot, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f + (camera->scale.d[0] - 1.0f)  * p, 1.0f + (camera->scale.d[1] - 1.0f)  * p, 1.0f);
    glTranslatef(-Point_getX(&camera->pos) * p, -Point_getY(&camera->pos) * p, 0.0f);

    
    Layer_render(layer); 
    
    
    glPopMatrix();    
}

void Renderer_render(Renderer* renderer)
{
    static float rot = 0;
    
    glLoadIdentity();
      
    Layer* layer;
    int i;
    for (i = 0; i < renderer->layercount; i++)
    {
        layer = ArrayList_get(renderer->layers, i);
        if (NULL != layer)
        {
            renderLayer(renderer, layer); 
        }
    }
    
    rot+=0.01f;
}

void Renderer_addDrawable(Renderer* renderer, Drawable* drawable)
{
    assert(NULL != renderer);
    assert(NULL != drawable);

    
    Layer* layer = ArrayList_get(renderer->layers, Drawable_getLayer(drawable));
    if (NULL == layer )
    {
        layer = Layer_new();
        ArrayList_set(renderer->layers, Drawable_getLayer(drawable), layer);
        renderer->layercount++;
    }
    Layer_addDrawable(layer, drawable);  
}


Camera* Renderer_getCamera(Renderer* renderer)
{
    assert(NULL != renderer);

    return renderer->camera;
}

void Renderer_setupLayer(Renderer* renderer, unsigned int id, float parallax)
{
    assert(NULL != renderer);
  
    Layer* layer = ArrayList_get(renderer->layers, id);
    if (NULL == layer )
    {
        layer = Layer_new();
        ArrayList_set(renderer->layers, id, layer);
        renderer->layercount++;
    }
    layer->parallax = parallax;
}


