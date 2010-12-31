#include <smugstd.h>
#include <common/common.h>
#include <common/log.h>
#include <platform/opengl/opengl.h>
#include <graphics/renderer/renderer.h>
#include <platform/platform.h>

#include "graphics.h"

Renderer* sceneRenderer = NULL;

int gVBOSupported = 0;
unsigned int gRenderMode = RENDER_NORMAL;

static BOOL isInitialized = FALSE;

static void printGLError(void)
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

static int setupGL(void)
{
#ifdef WIN32
    DEBUG("Initializing GLEW");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ERROR("GLEW could not be initialized, errcode: %s", glewGetErrorString(err));
        return 0;
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

    Vector screenSize = Platform_getWindowSize();
    Graphics_setWindowSize(screenSize.d[0], screenSize.d[1]);

	glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    printGLError();

    glEnable( GL_TEXTURE_2D );

    return 1;
}

void Graphics_setWindowSize(double w, double h)
{
    // glViewport(0, 0, (int)w, (int)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef SMUG_GLES
	glOrthof(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f);
#else
	glOrtho(0, w, h, 0, -1, 1);
#endif /* SMUG_GLES */

    glMatrixMode(GL_MODELVIEW);
}

int Graphics_init()
{
    smug_assert(!isInitialized);
	smug_assert(Platform_isInitialized());
	smug_assert(Platform_isWindowOpen());

    if (!setupGL())
        return 0;

    sceneRenderer = Renderer_new();

    isInitialized = TRUE;
    return 1;
}

BOOL Graphics_isInitialized(void)
{
    return isInitialized;
}

void Graphics_terminate(void)
{
    smug_assert(isInitialized);
    Renderer_delete(sceneRenderer);
    sceneRenderer = NULL;
    isInitialized = FALSE;
}

void Graphics_render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glLoadIdentity(); (called in Renderer_render)

    Renderer_render(sceneRenderer);
    // glFlush();
    // glFinish();
}

void Graphics_addDrawable(Drawable* d)
{
    Renderer_addDrawable(sceneRenderer, d);
}

void Graphics_removeDrawable(Drawable* d)
{
    WARNING("Unimplemented function Graphics_removeDrawable");
}

Camera* Graphics_getCamera()
{
    smug_assert(NULL != sceneRenderer);
    return Renderer_getCamera(sceneRenderer);

}

void Graphics_setupLayer(unsigned int id, float parallax)
{
    Renderer_setupLayer(sceneRenderer, id, parallax);
}

void Graphics_setRenderMode(unsigned int flags)
{
    gRenderMode = flags;
}
