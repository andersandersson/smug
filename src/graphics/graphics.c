#include "graphics.h"
#include "platform/opengl/opengl.h"
#include "renderer/renderer.h"

#include "common/log.h"

Renderer* layerRenderer = NULL;

int gVBOSupported = 0;
int gScreenWidth = 0;
int gScreenHeight = 0;

static void printGLError()
{
    int err = glGetError();

    switch (err)
    {
        case GL_INVALID_ENUM: 
            ERROR("GL error GL_INVALID_ENUM\n");
            break;
        case GL_INVALID_VALUE: 
            ERROR("GL error GL_INVALID_VALUE\n");
            break;
        case GL_INVALID_OPERATION: 
            ERROR("GL error GL_INVALID_OPERATION\n");
            break;
        case GL_STACK_OVERFLOW: 
            ERROR("GL error GL_STACK_OVERFLOW\n");
            break;
        case GL_STACK_UNDERFLOW: 
            ERROR("GL error GL_STACK_UNDERFLOW\n");
            break;
        case GL_OUT_OF_MEMORY: 
            ERROR("GL error GL_OUT_OF_MEMORY\n");
            break;   
        default:
            break;
    }
}


static void setupGL()
{
    DEBUG("Initializing GLEW");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ERROR("GLEW could not be initialized, errcode: %s", glewGetErrorString(err));
    }

    DEBUG("Checking for VBO support");
	if(GLEW_ARB_vertex_buffer_object)
	{   
        gVBOSupported = 1;
    }
    else
    {   
        WARNING("VBOs are not supported, rendering will use vertex arrays.\n");
    }

    DEBUG("Initializing GL");
    glDisable(GL_CULL_FACE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, gScreenWidth, gScreenHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    printGLError();
   
    //glEnable( GL_TEXTURE_2D );
}

int Graphics_init(int width, int height)
{
    gScreenWidth = width;
    gScreenHeight = height;
    DEBUG("Graphics resolution set to %ix%i", width, height);
    
    setupGL();
   
    layerRenderer = Renderer_new();

    return 1;
}

void Graphics_terminate()
{

    Renderer_delete(layerRenderer);
}

void Graphics_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    Renderer_render(layerRenderer);
}


void Graphics_addDrawable(Drawable* d)
{
    Renderer_addDrawable(layerRenderer, d);
}


void Graphics_fillRect(Rectangle* rect, Color* color)
{
    assert(NULL != rect);
    assert(NULL != color);
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor4f(color->r, color->g, color->b, color->a);
    glBegin(GL_QUADS);
        glVertex3f(rect->x,           rect->y,           0.0f);
        glVertex3f(rect->x + rect->w, rect->y,           0.0f);
        glVertex3f(rect->x + rect->w, rect->y + rect->h, 0.0f);
        glVertex3f(rect->x,           rect->y + rect->h, 0.0f);
    glEnd();
    glPopAttrib();
}
