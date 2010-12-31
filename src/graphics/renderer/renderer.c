#include "renderer.h"

#include <math.h>

#include "renderbatch.h"
#include "layer.h"

#include <stdlib.h>
#include <stdio.h>

#include "graphics/graphics.h"
#include "graphics/sprite.h"

#include <platform/platform.h>
#include <platform/opengl/opengl.h>

extern int gVBOSupported;
extern unsigned int gRenderMode;

Renderer* Renderer_new(void)
{
    Renderer* ret = (Renderer*)malloc(sizeof(Renderer));
    ret->layers = ArrayList_new();
    ret->layercount = 0;
    // ret->camera = Camera_new(Vector_multiply(Platform_getWindowSize(), 0.5));
    ret->camera = Camera_new(Vector_create2d(0.0, 0.0));

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

    // TODO: Fix zoom thingies
    glScalef(1.0f + (camera->scale.d[0] - 1.0f)  * p, 1.0f + (camera->scale.d[1] - 1.0f)  * p, 1.0f);
    glTranslatef(-Point_getX(&camera->pos) * p, -Point_getY(&camera->pos) * p, 0.0f);

    Layer_render(layer);

    glPopMatrix();
}

static void renderDebugGrid(Renderer* renderer)
{
// TODO: Implement this function without using openGL's immediate mode.
#ifndef SMUG_GLES
    Camera* camera = renderer->camera;

    glPushMatrix();

    float scalex = camera->scale.d[0];
    float scaley = camera->scale.d[1];
    glScalef(scalex, scaley, 1.0f);
    glTranslatef(-Point_getX(&camera->pos), -Point_getY(&camera->pos), 0.0f);
    Vector screenSize = Vector_multiply(Platform_getWindowSize(), 0.5);
    float gridsizex = 16.0f;
    float gridsizey = 16.0f;
    float cposx = Point_getX(&camera->pos);
    float cposy = Point_getY(&camera->pos);
    float offsetx = cposx - (((int)(cposx / gridsizex)) * gridsizex);
    float offsety = cposy - (((int)(cposy / gridsizey)) * gridsizey);
    float ratio = screenSize.d[0] / screenSize.d[1] - 1;
    float cx = cposx - offsetx;
    float cw = (screenSize.d[0]) / scalex;
    float cy = cposy - offsety;
    float ch = (screenSize.d[1]) / scaley;

    // Draw grid outwards from the camera center.
    // Adjust the size of the grid to cover the entire view.
    // TODO: Compensate for screen ratio when rotated.
    glDisable(GL_TEXTURE_2D); // TODO: dont allways disable textures here, try to check if its already disabled
    glBegin(GL_LINES);
    {
        float i;
        // Draw vertical lines from the middle to the right
        for (i = cx; i <= cx + cw + gridsizex; i+=gridsizex)
        {
            glColor4f(1.0f - (abs(i)/32.0f),(abs(i)/32.0f),(abs(i)/640.0f),scalex/2.0f);
            glVertex2f(i, cy - ch + offsety);
            glVertex2f(i, cy + ch + offsety);
        }
        // Draw vertical lines from the middle to the left
        for (i = cx - gridsizex; i >= cx - cw - gridsizex; i-=gridsizex)
        {
            glColor4f(1.0f - (abs(i)/32.0f),(abs(i)/32.0f),(abs(i)/640.0f),scalex/2.0f);
            glVertex2f(i, cy - ch + offsety);
            glVertex2f(i, cy + ch + offsety);
        }
        // Draw horizontal lines from the middle to the botton
        for (i = cy; i <= cy + ch + gridsizex; i+=gridsizey)
        {
            glColor4f(1.0f - (abs(i)/32.0f),(abs(i)/32.0f),(abs(i)/640.0f),scaley/2.0f);
            glVertex2f(cx - cw + offsetx, i);
            glVertex2f(cx + cw + offsetx, i);
        }
        // Draw horizontal lines from the middle to the top
        for (i = cy - gridsizey; i >= cy - ch - gridsizey; i-=gridsizey)
        {
            glColor4f(1.0f - (abs(i)/32.0f),(abs(i)/32.0f),(abs(i)/640.0f),scaley/2.0f);
            glVertex2f(cx - cw + offsetx, i);
            glVertex2f(cx + cw + offsetx, i);
        }
    }
    glEnd();

    glPopMatrix();
#endif /* SMUG_GLES */
}

void Renderer_render(Renderer* renderer)
{
    glLoadIdentity();

    Camera* camera = renderer->camera;
    glTranslatef(camera->center.d[0], camera->center.d[1], 0.0f);
    glRotatef(renderer->camera->rot, 0.0f, 0.0f, 1.0f);

    if (gRenderMode & RENDER_LAYERS)
    {
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
    }

    if (gRenderMode & RENDER_DEBUG_GRID)
    {
        renderDebugGrid(renderer);
    }

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
