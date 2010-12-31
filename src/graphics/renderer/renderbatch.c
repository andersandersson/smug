#include "renderbatch.h"

#include <smugstd.h>

#include "common/log.h"

#include "graphics/texture/texture.h"
#include "platform/opengl/opengl.h"


extern int gVBOSupported;

static void generateBuffer(GLuint* idholder)
{
    *idholder = 0;
    if (gVBOSupported)
    {
        glGenBuffers(1, idholder);
        if (!glIsBuffer(*idholder))
        {
            WARNING("Failed to create buffer, err: %i. Disabling VBOs.\n",  glGetError());
            gVBOSupported = 0;

        }
    }
}

static void freeBuffer(GLuint* idholder)
{
    if (glIsBuffer(*idholder))
        glDeleteBuffers(1, idholder);
}

RenderBatch* RenderBatch_new(unsigned int objectSize, Texture* texture, unsigned int initialSize)
{
    RenderBatch* ret = (RenderBatch*)malloc(sizeof(RenderBatch));

    ret->data = BatchData_new(initialSize);

    ret->objectSize = objectSize;
    ret->dataSize = 0;
    ret->texture = texture;
    ret->drawables = LinkedList_new();

    generateBuffer(&ret->vertexBufferIndex);
    generateBuffer(&ret->colorBufferIndex);
    generateBuffer(&ret->textureBufferIndex);


    return ret;
}

void RenderBatch_delete(void* batch)
{
    assert(NULL != batch);

    RenderBatch* thebatch = (RenderBatch*)batch;
    BatchData_delete(thebatch->data);

    freeBuffer(&thebatch->vertexBufferIndex);
    freeBuffer(&thebatch->colorBufferIndex);
    freeBuffer(&thebatch->textureBufferIndex);

    // Delete only list, don't touch drawables
    LinkedList_delete(thebatch->drawables);

    free(thebatch);
}


unsigned int RenderBatch_getSize(RenderBatch* batch)
{
    assert(NULL != batch);
    return batch->dataSize;
}

void RenderBatch_addDrawable(RenderBatch* batch, Drawable* drawable)
{
    assert(NULL != batch);
    assert(NULL != drawable);
    assert(Drawable_getTexture(drawable) == batch->texture);
    assert(Drawable_getObjectSize(drawable) == batch->objectSize);

    // Add drawable to list
    LinkedList_addLast(batch->drawables, drawable);
}
unsigned int RenderBatch_getDrawableCount(RenderBatch* batch)
{
    return LinkedList_length(batch->drawables);

}

void RenderBatch_write(RenderBatch* currentBatch)
{
    // Write batch vertex data for all drawables

    // Make sure batch can contain all data we want to write
    unsigned int expectedSize = RenderBatch_getDrawableCount(currentBatch) * currentBatch->objectSize;
    if (expectedSize > BatchData_getCapacity(currentBatch->data))
    {
        BatchData_growCapacityTo(currentBatch->data, expectedSize);
    }

    int writepos = 0;
    unsigned int capacity = BatchData_getCapacity(currentBatch->data);
    unsigned int datasize;
    Node* node;
    node = currentBatch->drawables->first;
    while (NULL != node)
    {
        // find out how much data space the drawable needs
        datasize = Drawable_getDataSize(node->item);

        // if the amount of data cannot fit, expand batch
        if (writepos + Drawable_getDataSize(node->item) > capacity)
        {
            BatchData_expandTo(currentBatch->data, capacity + datasize);
        }
        Drawable_writeBatchData(node->item, currentBatch->data, writepos);
        writepos += datasize;

        node = node->next;
    }
    currentBatch->dataSize = writepos;

}

void RenderBatch_render(RenderBatch* currentBatch)
{
    static BOOL texture_enabled = FALSE;

    int tex = 0;
    if (NULL != currentBatch->texture)
    {
        if (texture_enabled != TRUE)
        {
            glEnable(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, currentBatch->texture->texid);
    }
    else
    {
        if (texture_enabled != FALSE)
        {
            glDisable(GL_TEXTURE_2D);
        }
    }

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    unsigned int batchSize = currentBatch->dataSize;
    if(gVBOSupported)
    {
        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->vertexBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, batchSize * 2 * sizeof(float), currentBatch->data->vertexData, GL_STATIC_DRAW );
        glVertexPointer( 2, GL_FLOAT, 0, (char*) NULL );

        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->textureBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, batchSize * 2 * sizeof(float), currentBatch->data->textureData, GL_STATIC_DRAW );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char*) NULL );

        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->colorBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, batchSize * 4 * sizeof(float), currentBatch->data->colorData, GL_STATIC_DRAW );
        glColorPointer(4, GL_FLOAT, 0, (char*) NULL );

    } else
    {
        glVertexPointer( 2, GL_FLOAT, 0, currentBatch->data->vertexData );	// Set The Vertex Pointer To Our Vertex Data
        glTexCoordPointer( 2, GL_FLOAT, 0, currentBatch->data->textureData );
        glColorPointer(4, GL_FLOAT, 0, currentBatch->data->colorData);
    }

    switch (currentBatch->objectSize)
    {
        case 1:
        {
            glDrawArrays(GL_POINTS, 0, batchSize);
            break;
        }
        case 2:
        {
            glDrawArrays(GL_LINES, 0, batchSize);
            break;
        }
        case 3:
        case 6:
        {
            glDrawArrays(GL_TRIANGLES, 0, batchSize);
            break;
        }
#ifndef SMUG_GLES
        case 4:
        {
            glDrawArrays(GL_QUADS, 0, batchSize);
            break;
        }
#endif /* SMUG_GLES */
        default:
        {
            glDrawArrays(GL_TRIANGLE_FAN, 0, batchSize);
        }
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

/*
    glBegin(GL_QUADS);
    {
        int i;
        for (i = 0; i < renderer->drawables->length; i++)
        {
            glColor4f(currentBatch->colorData[i * currentBatch->objectSize * 4 + 0], currentBatch->colorData[i * currentBatch->objectSize * 4 + 1],
                        currentBatch->colorData[i * currentBatch->objectSize * 4 + 2], currentBatch->colorData[i * currentBatch->objectSize * 4 + 3]);

            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 0],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 1]);
            glTexCoord2f(currentBatch->textureData[i * currentBatch->objectSize * 2 + 0],currentBatch->textureData[i * currentBatch->objectSize * 2 + 1]);

            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 2],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 3]);
            glTexCoord2f(currentBatch->textureData[i * currentBatch->objectSize * 2 + 2],currentBatch->textureData[i * currentBatch->objectSize * 2 + 3]);

            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 4],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 5]);
            glTexCoord2f(currentBatch->textureData[i * currentBatch->objectSize * 2 + 4],currentBatch->textureData[i * currentBatch->objectSize * 2 + 5]);

            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 6],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 7]);
            glTexCoord2f(currentBatch->textureData[i * currentBatch->objectSize * 2 + 6],currentBatch->textureData[i * currentBatch->objectSize * 2 + 7]);

            fprintf(stderr, "drawing box x1:%i, y1:%i x2:%i, y2:%i\n", (int)currentBatch->vertexData[i * currentBatch->objectSize * 2 + 0], (int)currentBatch->vertexData[i * currentBatch->objectSize * 2 + 1], (int)currentBatch->vertexData[i * currentBatch->objectSize * 2 + 4], (int)currentBatch->vertexData[i * currentBatch->objectSize * 2 + 5]);
            fprintf(stderr, " with texture:%i, tx1:%i, ty1:%i tx2:%i, ty2:%i\n", tex, (int)currentBatch->textureData[i * currentBatch->objectSize * 2 + 0], (int)currentBatch->textureData[i * currentBatch->objectSize * 2 + 1], (int)currentBatch->textureData[i * currentBatch->objectSize * 2 + 4], (int)currentBatch->textureData[i * currentBatch->objectSize * 2 + 5]);

        }
    }
    glEnd();
    */
}

