#include "graphics.h"
#include "platform/opengl/opengl.h"
#include "renderer/renderer.h"

#include "common/log.h"

Renderer* sceneRenderer = NULL;

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
#ifdef WIN32
    DEBUG("Initializing GLEW");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ERROR("GLEW could not be initialized, errcode: %s", glewGetErrorString(err));
    }
#endif //WIN32
    
    DEBUG("Checking for VBO support");
	if(GL_ARB_vertex_buffer_object)
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    printGLError();
   
    glEnable( GL_TEXTURE_2D );
}

int Graphics_init(int width, int height)
{
	assert(Platform_isInitialized());
	assert(Platform_isWindowOpen());

    gScreenWidth = width;
    gScreenHeight = height;
    DEBUG("Graphics resolution set to %ix%i", width, height);
    
    setupGL();
   
    sceneRenderer = Renderer_new();

    return 1;
}

void Graphics_terminate()
{

    Renderer_delete(sceneRenderer);
}

void Graphics_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    Renderer_render(sceneRenderer);
}


void Graphics_addDrawable(Drawable* d)
{
    Renderer_addDrawable(sceneRenderer, d);
}

Camera* Graphics_getCamera()
{
    assert(NULL != sceneRenderer);
    return Renderer_getCamera(sceneRenderer);

}

void Graphics_setupLayer(unsigned int id, float parallax)
{
    Renderer_setupLayer(sceneRenderer, id, parallax);
}
