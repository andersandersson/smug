#include "renderer.h"

#include "platform/opengl/opengl.h"
#include "renderbatch.h"

#include "stdlib.h"
#include "stdio.h"

#define DEFAULT_BATCHSIZE 100

extern int gVBOSupported;

Renderer* Renderer_new()
{
    Renderer* ret = (Renderer*)malloc(sizeof(Renderer));
    ret->drawables = LinkedList_new();
    ret->quadBatches = LinkedList_new();
    ret->size = 0;
    
    LinkedList_addLast(ret->quadBatches, RenderBatch_new(BATCHTYPE_QUADS, DEFAULT_BATCHSIZE));
    
    return ret;
}

void Renderer_delete(void* renderer)
{
    LinkedList_deleteContents(((Renderer*)renderer)->drawables, Drawable_delete);
    LinkedList_delete(((Renderer*)renderer)->drawables);
    
    LinkedList_deleteContents(((Renderer*)renderer)->quadBatches, RenderBatch_delete);
    LinkedList_delete(((Renderer*)renderer)->quadBatches); 
}

void Renderer_render(Renderer* renderer)
{
    RenderBatch* currentBatch = renderer->quadBatches->first->item;
    
    // Set the current batch capacity in vertices
    RenderBatch_setCapacity(currentBatch, renderer->drawables->length * currentBatch->objectSize);
    
    // write batch vertex data for all boxes
    Node* node;
    node = renderer->drawables->first;
    int writepos = 0;
    while (NULL != node)
    {
        writepos += Drawable_writeBatchData(node->item, currentBatch, writepos);
        
        node = node->next;
    }

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);	
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);	   
    

    if(gVBOSupported)
    {    
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentBatch->vertexBufferIndex);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, renderer->drawables->length * currentBatch->objectSize*2*sizeof(float), currentBatch->vertexData, GL_STATIC_DRAW_ARB );
        glVertexPointer( 2, GL_FLOAT, 0, (char*) NULL );
/*
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentBatch->textureBufferIndex);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, BOXES*currentBatch->objectSize*2*sizeof(float), currentBatch->textureData, GL_STATIC_DRAW_ARB );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char*) NULL );
 */       
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentBatch->colorBufferIndex);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, renderer->drawables->length * currentBatch->objectSize*4*sizeof(float), currentBatch->colorData, GL_STATIC_DRAW_ARB );
        glColorPointer(4, GL_FLOAT, 0, (char*) NULL );
     
    } else
    {
        glVertexPointer( 2, GL_FLOAT, 0, currentBatch->vertexData );	// Set The Vertex Pointer To Our Vertex Data
        glTexCoordPointer( 2, GL_FLOAT, 0, currentBatch->textureData );
        glColorPointer(4, GL_FLOAT, 0, currentBatch->colorData);
    }
    
    glDrawArrays(GL_QUADS, 0, renderer->drawables->length * currentBatch->objectSize);     
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);	    
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);	 

    /*glColor4f(1, 1, 1, 0.01f);
    glBegin(GL_QUADS);
    {
        int i;
        for (i = 0; i < renderer->drawables->length; i++)
        {
            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 0],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 1]);
            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 2],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 3]);
            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 4],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 5]);       
            glVertex2f(currentBatch->vertexData[i * currentBatch->objectSize * 2 + 6],currentBatch->vertexData[i * currentBatch->objectSize * 2 + 7]);     
            //fprintf(stderr, "drawing box x1:%i, y1:%i x2:%i, y2:%i\n", currentBatch->vertexData[i * currentBatch->objectSize * 2 + 0], currentBatch->vertexData[i * currentBatch->objectSize * 2 + 1], currentBatch->vertexData[i * currentBatch->objectSize * 2 + 4], currentBatch->vertexData[i * currentBatch->objectSize * 2 + 5]);
        }
    }
    glEnd();   */
            
}

void Renderer_addDrawable(Renderer* renderer, Drawable* d)
{

    LinkedList_addLast(renderer->drawables, d);
}
