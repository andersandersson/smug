#include "renderbatch.h"
#include "platform/opengl/opengl.h"

#include "stdlib.h"

#include "common/log.h"

extern int gVBOSupported;


static void generateBuffer(GLuint* idholder)
{
    *idholder = 0;
    if (gVBOSupported)
    {
        glGenBuffersARB(1, idholder); 
        if (!glIsBufferARB(*idholder))
        {
            WARNING("Failed to create buffer, err: %i. Disabling VBOs.\n",  glGetError());
            gVBOSupported = 0;

        }
    }
}

static void freeBuffer(GLuint* idholder)
{
    if (glIsBufferARB(*idholder))
        glDeleteBuffersARB(1, idholder);
}

RenderBatch* RenderBatch_new(unsigned int objectSize, unsigned int vertices)
{
    RenderBatch* ret = (RenderBatch*)malloc(sizeof(RenderBatch));
     
    ret->vertexData = malloc(sizeof(float) * vertices * 2);
    ret->colorData = malloc(sizeof(float) * vertices * 4);
    ret->textureData = malloc(sizeof(float) * vertices * 2);
    generateBuffer(&ret->vertexBufferIndex);
    generateBuffer(&ret->colorBufferIndex);
    generateBuffer(&ret->textureBufferIndex);
    ret->_capacity = vertices;
    ret->objectSize = objectSize;
    return ret;
}

void RenderBatch_delete(void* batch)
{
    if (NULL != ((RenderBatch*)batch)->vertexData)
        free(((RenderBatch*)batch)->vertexData);
        
    if (NULL != ((RenderBatch*)batch)->colorData)      
        free(((RenderBatch*)batch)->colorData);
        
    if (NULL != ((RenderBatch*)batch)->textureData)
        free(((RenderBatch*)batch)->textureData);
    
    ((RenderBatch*)batch)->vertexData = NULL;
    ((RenderBatch*)batch)->colorData = NULL;
    ((RenderBatch*)batch)->textureData = NULL;   
    
    freeBuffer(((RenderBatch*)batch)->vertexBufferIndex);
    freeBuffer(((RenderBatch*)batch)->colorBufferIndex);
    freeBuffer(((RenderBatch*)batch)->textureBufferIndex);
}


void RenderBatch_setCapacity(RenderBatch* batch, unsigned int vertices)
{
    if (vertices <= batch->_capacity)
        return;
    
    free(batch->vertexData);
    free(batch->colorData);
    free(batch->textureData);
    
    batch->vertexData = malloc(sizeof(GLfloat) * vertices * 2);
    batch->colorData = malloc(sizeof(GLfloat) * vertices * 4);
    batch->textureData = malloc(sizeof(GLfloat) * vertices * 2);     
    
    batch->_capacity = vertices;
}

unsigned int RenderBatch_getCapacity(RenderBatch* batch)
{
    return batch->_capacity;
}



