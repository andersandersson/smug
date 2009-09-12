#include "renderer.h"

#include "platform/opengl/opengl.h"
#include "renderbatch.h"

#include "stdlib.h"
#include "stdio.h"

#define DEFAULT_BATCHSIZE 100

#include "graphics/sprite.h"

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
    
    
    int tex = 0;
    
    // write batch vertex data for all boxes
    Node* node;
    node = renderer->drawables->first;   
    if (((Drawable*)node->item)->sprite)
        tex = ((Drawable*)node->item)->sprite->texture->texid;
    
    int writepos = 0;
    while (NULL != node)
    {
        writepos += Drawable_writeBatchData(node->item, currentBatch, writepos);
        
        node = node->next;
    }
    
    glBindTexture(GL_TEXTURE_2D, tex);
    

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);	
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);	   
    
    if(gVBOSupported)
    {    
        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->vertexBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, renderer->drawables->length * currentBatch->objectSize*2*sizeof(float), currentBatch->vertexData, GL_STATIC_DRAW );
        glVertexPointer( 2, GL_FLOAT, 0, (char*) NULL );

        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->textureBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, renderer->drawables->length * currentBatch->objectSize*2*sizeof(float), currentBatch->textureData, GL_STATIC_DRAW );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char*) NULL );
        
        glBindBuffer(GL_ARRAY_BUFFER, currentBatch->colorBufferIndex);
        glBufferData(GL_ARRAY_BUFFER, renderer->drawables->length * currentBatch->objectSize*4*sizeof(float), currentBatch->colorData, GL_STATIC_DRAW );
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

void Renderer_addDrawable(Renderer* renderer, Drawable* d)
{

    LinkedList_addLast(renderer->drawables, d);
}
