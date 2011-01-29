#ifndef SMUG_GRAPHICS_INTERNAL
#define SMUG_GRAPHICS_INTERNAL

#ifdef SMUG_DEBUG
#define printGLError() \
do \
{ \
    int err = glGetError(); \
    while (err != GL_NO_ERROR) \
    { \
        switch (err) \
        { \
            case GL_INVALID_ENUM: \
                ERROR("GL error GL_INVALID_ENUM"); \
                break; \
            case GL_INVALID_VALUE: \
                ERROR("GL error GL_INVALID_VALUE"); \
                break; \
            case GL_INVALID_OPERATION: \
                ERROR("GL error GL_INVALID_OPERATION"); \
                break; \
            case GL_STACK_OVERFLOW: \
                ERROR("GL error GL_STACK_OVERFLOW"); \
                break; \
            case GL_STACK_UNDERFLOW: \
                ERROR("GL error GL_STACK_UNDERFLOW"); \
                break; \
            case GL_OUT_OF_MEMORY: \
                ERROR("GL error GL_OUT_OF_MEMORY"); \
                break; \
            default: \
                ERROR("GL error of unknown type: %i", err); \
                break; \
        } \
        err = glGetError(); \
    } \
} while(0)
#endif
#ifndef SMUG_DEBUG
#define printGLError() SMUG_NOOP()
#endif

#endif /* SMUG_GRAPHICS_INTERNAL */
