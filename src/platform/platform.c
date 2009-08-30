#include "platform.h"

#include "GL/glfw.h"
#include "common/log.h"

static int keymap[KEY_LAST];

static void mapKeys()
{
    int i;
    for (i = 0; i < KEY_SPECIAL; i++)
    {
        keymap[i] = i;
    }

    keymap[KEY_UNKNOWN] = GLFW_KEY_UNKNOWN;  
    keymap[KEY_SPACE] = GLFW_KEY_SPACE;  
    keymap[KEY_SPECIAL] = GLFW_KEY_SPECIAL;  
    keymap[KEY_ESC] = GLFW_KEY_ESC;  
    keymap[KEY_F1] = GLFW_KEY_F1;  
    keymap[KEY_F2] = GLFW_KEY_F2;  
    keymap[KEY_F3] = GLFW_KEY_F3;  
    keymap[KEY_F4] = GLFW_KEY_F4;  
    keymap[KEY_F5] = GLFW_KEY_F5;  
    keymap[KEY_F6] = GLFW_KEY_F6;  
    keymap[KEY_F7] = GLFW_KEY_F7;  
    keymap[KEY_F8] = GLFW_KEY_F8;  
    keymap[KEY_F9] = GLFW_KEY_F9;  
    keymap[KEY_F10] = GLFW_KEY_F10;  
    keymap[KEY_F11] = GLFW_KEY_F11;  
    keymap[KEY_F12] = GLFW_KEY_F12;  
    keymap[KEY_F13] = GLFW_KEY_F13;  
    keymap[KEY_F14] = GLFW_KEY_F14;  
    keymap[KEY_F15] = GLFW_KEY_F15;  
    keymap[KEY_F16] = GLFW_KEY_F16;  
    keymap[KEY_F17] = GLFW_KEY_F17;  
    keymap[KEY_F18] = GLFW_KEY_F18;  
    keymap[KEY_F19] = GLFW_KEY_F19;  
    keymap[KEY_F20] = GLFW_KEY_F20;  
    keymap[KEY_F21] = GLFW_KEY_F21;  
    keymap[KEY_F22] = GLFW_KEY_F22;  
    keymap[KEY_F23] = GLFW_KEY_F23;  
    keymap[KEY_F24] = GLFW_KEY_F24;  
    keymap[KEY_F25] = GLFW_KEY_F25;    
    keymap[KEY_UP] = GLFW_KEY_UP;  
    keymap[KEY_DOWN] = GLFW_KEY_DOWN;  
    keymap[KEY_LEFT] = GLFW_KEY_LEFT;  
    keymap[KEY_RIGHT] = GLFW_KEY_RIGHT;  
    keymap[KEY_LSHIFT] = GLFW_KEY_LSHIFT;  
    keymap[KEY_RSHIFT] = GLFW_KEY_RSHIFT;  
    keymap[KEY_LCTRL] = GLFW_KEY_LCTRL;  
    keymap[KEY_RCTRL] = GLFW_KEY_RCTRL;  
    keymap[KEY_LALT] = GLFW_KEY_LALT;  
    keymap[KEY_RALT] = GLFW_KEY_RALT;  
    keymap[KEY_TAB] = GLFW_KEY_TAB;  
    keymap[KEY_ENTER] = GLFW_KEY_ENTER;  
    keymap[KEY_BACKSPACE] = GLFW_KEY_BACKSPACE;  
    keymap[KEY_INSERT] = GLFW_KEY_INSERT;  
    keymap[KEY_DEL] = GLFW_KEY_DEL;  
    keymap[KEY_PAGEUP] = GLFW_KEY_PAGEUP;  
    keymap[KEY_PAGEDOWN] = GLFW_KEY_PAGEDOWN;  
    keymap[KEY_HOME] = GLFW_KEY_HOME;  
    keymap[KEY_END] = GLFW_KEY_END;  
    keymap[KEY_KP_0] = GLFW_KEY_KP_0;  
    keymap[KEY_KP_1] = GLFW_KEY_KP_1;
    keymap[KEY_KP_2] = GLFW_KEY_KP_2;
    keymap[KEY_KP_3] = GLFW_KEY_KP_3;
    keymap[KEY_KP_4] = GLFW_KEY_KP_4;
    keymap[KEY_KP_5] = GLFW_KEY_KP_5;
    keymap[KEY_KP_6] = GLFW_KEY_KP_6;
    keymap[KEY_KP_7] = GLFW_KEY_KP_7;
    keymap[KEY_KP_8] = GLFW_KEY_KP_8;
    keymap[KEY_KP_9] = GLFW_KEY_KP_9;
    keymap[KEY_KP_DIVIDE] = GLFW_KEY_KP_DIVIDE;  
    keymap[KEY_KP_MULTIPLY] = GLFW_KEY_KP_MULTIPLY;  
    keymap[KEY_KP_SUBTRACT] = GLFW_KEY_KP_SUBTRACT;  
    keymap[KEY_KP_ADD] = GLFW_KEY_KP_ADD;  
    keymap[KEY_KP_DECIMAL] = GLFW_KEY_KP_DECIMAL;  
    keymap[KEY_KP_EQUAL] = GLFW_KEY_KP_EQUAL;  
    keymap[KEY_KP_ENTER] = GLFW_KEY_KP_ENTER;
}

int Platform_init()
{
    NOTIFY("Initializing platform layer");
    glfwInit();
    mapKeys();
}
void Platform_terminate()
{
    NOTIFY("Terminating platform layer");
    glfwTerminate();
}

BOOL Platform_openWindow(int width, int height, BOOL fullscreen)
{
    return glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, fullscreen? GLFW_FULLSCREEN : GLFW_WINDOW);
}

void Platform_closeWindow()
{
    glfwCloseWindow();
}

BOOL Platform_isWindowOpen()
{
    return glfwGetWindowParam(GLFW_OPENED);
}

void Platform_refreshWindow()
{
    glfwSwapBuffers();
}

TIME Platform_getTime()
{
    return glfwGetTime();
}

void Platform_sleep(TIME seconds)
{
    glfwSleep(seconds);
}

BOOL Platform_getKey(int key)
{
    return glfwGetKey(keymap[key]);
}

