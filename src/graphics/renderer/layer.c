#include "layer.h"

#include "stdlib.h"

#include "graphics/renderer/renderbatch.h"
#include "graphics/texture/texture.h"
#include "graphics/sprite.h"



#define OBJECTSIZE_START 4
#define TEXTURECOUNT_START 4
#define VERTEXCOUNT_START 100

Layer* Layer_new()
{
    Layer* ret = (Layer*)malloc(sizeof(Layer*));
    // Create space for objectsizes up to OBJECTSIZE_START
    ret->batcharrays = ArrayList_newFromCapacity(OBJECTSIZE_START); 
    ret->parallax = 1.0f;
    return ret;
}


static void batcharrayDelete(void* batcharray)
{   
    if (NULL != batcharray)
    {
        ArrayList_deleteContents((ArrayList*)batcharray, RenderBatch_delete);
        ArrayList_delete(batcharray);
    }
}


void Layer_delete(void* layer)
{
    Layer* thelayer = (Layer*)layer;
    // Delete all arrays of batches
    ArrayList_deleteContents(thelayer->batcharrays, batcharrayDelete);
    ArrayList_delete(thelayer->batcharrays);
    
    free(thelayer);
}

void Layer_addDrawable(Layer* layer, Drawable* drawable)
{
    assert(NULL != layer);
    assert(NULL != drawable);
    
    // find the proper batch to put the drawable in

    unsigned int osize = Drawable_getObjectSize(drawable);
   
    // Check object size and extend batcharrays if needed
    ArrayList* batcharray = ArrayList_get(layer->batcharrays, osize);
    if (NULL == batcharray)
    {
        // Create batcharray sorted by texture
        batcharray = ArrayList_newFromCapacity(TEXTURECOUNT_START);
        ArrayList_set(layer->batcharrays, osize, batcharray); 
    }
    
    // Check texture id and extend batcharray if needed
    Texture* texture = Drawable_getTexture(drawable);
    unsigned int texid = Drawable_getTextureID(drawable);
    RenderBatch* batch = ArrayList_get(batcharray, texid);
    if (NULL == batch)
    {
        // Create batch
        batch = RenderBatch_new(osize, texture, VERTEXCOUNT_START);
        ArrayList_set(batcharray, texid, batch); 
    } 
    
    // Add the drawable to batch
    RenderBatch_addDrawable(batch, drawable);
}

static void doForEachBatch(Layer* layer, void (*batchFunc)(RenderBatch*))
{
    int i;
    int j;
    ArrayList* batcharray;
    RenderBatch* batch;
    for (i = 0; i < ArrayList_getCapacity(layer->batcharrays); i++)
    {
        batcharray = ArrayList_get(layer->batcharrays, i);
        if (NULL != batcharray)
        {
            for (j = 0; j < ArrayList_getCapacity(batcharray); j++)
            {
                batch = ArrayList_get(batcharray, j);
                if (NULL != batch)
                {
                    batchFunc(batch);
                }
            }
        }
    }
}


void Layer_render(Layer* layer)
{
    assert(NULL != layer);
 
    // write all batches
    doForEachBatch(layer, RenderBatch_write);
 
    // render all batches
    doForEachBatch(layer, RenderBatch_render);
    
}

float Layer_getParallax(Layer* layer)
{
    assert(NULL != layer);
    return layer->parallax;
}
void Layer_setParallax(Layer* layer, float value)
{
    assert(NULL != layer);
    layer->parallax = value;
}



