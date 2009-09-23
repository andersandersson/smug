#include "platform.h"

#include "GL/glfw.h"
#include "common/log.h"
#include "stdlib.h"

static BOOL isInitialized = FALSE;

static int keymap[GLFW_KEY_LAST];
static int old_mousex = 0;
static int old_mousey = 0;
static int old_wheel = 0;

static void (*inputHandler)(int device, int trigger, INPUTSTATE state) = NULL;

static void mapKeys()
{
    int i;
    for (i = 0; i < GLFW_KEY_SPECIAL; i++)
    {
        keymap[i] = i;
    }
  
    keymap[GLFW_KEY_SPACE] = KEY_SPACE;  
    keymap[GLFW_KEY_SPECIAL] = KEY_SPECIAL;  
    keymap[GLFW_KEY_ESC] = KEY_ESC;  
    keymap[GLFW_KEY_F1] = KEY_F1;  
    keymap[GLFW_KEY_F2] = KEY_F2;  
    keymap[GLFW_KEY_F3] = KEY_F3;  
    keymap[GLFW_KEY_F4] = KEY_F4;  
    keymap[GLFW_KEY_F5] = KEY_F5;  
    keymap[GLFW_KEY_F6] = KEY_F6;  
    keymap[GLFW_KEY_F7] = KEY_F7;  
    keymap[GLFW_KEY_F8] = KEY_F8;  
    keymap[GLFW_KEY_F9] = KEY_F9;  
    keymap[GLFW_KEY_F10] = KEY_F10;  
    keymap[GLFW_KEY_F11] = KEY_F11;  
    keymap[GLFW_KEY_F12] = KEY_F12;  
    keymap[GLFW_KEY_F13] = KEY_F13;  
    keymap[GLFW_KEY_F14] = KEY_F14;  
    keymap[GLFW_KEY_F15] = KEY_F15;  
    keymap[GLFW_KEY_F16] = KEY_F16;  
    keymap[GLFW_KEY_F17] = KEY_F17;  
    keymap[GLFW_KEY_F18] = KEY_F18;  
    keymap[GLFW_KEY_F19] = KEY_F19;  
    keymap[GLFW_KEY_F20] = KEY_F20;  
    keymap[GLFW_KEY_F21] = KEY_F21;  
    keymap[GLFW_KEY_F22] = KEY_F22;  
    keymap[GLFW_KEY_F23] = KEY_F23;  
    keymap[GLFW_KEY_F24] = KEY_F24;  
    keymap[GLFW_KEY_F25] = KEY_F25;    
    keymap[GLFW_KEY_UP] = KEY_UP;  
    keymap[GLFW_KEY_DOWN] = KEY_DOWN;  
    keymap[GLFW_KEY_LEFT] = KEY_LEFT;  
    keymap[GLFW_KEY_RIGHT] = KEY_RIGHT;  
    keymap[GLFW_KEY_LSHIFT] = KEY_LSHIFT;  
    keymap[GLFW_KEY_RSHIFT] = KEY_RSHIFT;  
    keymap[GLFW_KEY_LCTRL] = KEY_LCTRL;  
    keymap[GLFW_KEY_RCTRL] = KEY_RCTRL;  
    keymap[GLFW_KEY_LALT] = KEY_LALT;  
    keymap[GLFW_KEY_RALT] = KEY_RALT;  
    keymap[GLFW_KEY_TAB] = KEY_TAB;  
    keymap[GLFW_KEY_ENTER] = KEY_ENTER;  
    keymap[GLFW_KEY_BACKSPACE] = KEY_BACKSPACE;  
    keymap[GLFW_KEY_INSERT] = KEY_INSERT;  
    keymap[GLFW_KEY_DEL] = KEY_DEL;  
    keymap[GLFW_KEY_PAGEUP] = KEY_PAGEUP;  
    keymap[GLFW_KEY_PAGEDOWN] = KEY_PAGEDOWN;  
    keymap[GLFW_KEY_HOME] = KEY_HOME;  
    keymap[GLFW_KEY_END] = KEY_END;  
    keymap[GLFW_KEY_KP_0] = KEY_KP_0;  
    keymap[GLFW_KEY_KP_1] = KEY_KP_1;
    keymap[GLFW_KEY_KP_2] = KEY_KP_2;
    keymap[GLFW_KEY_KP_3] = KEY_KP_3;
    keymap[GLFW_KEY_KP_4] = KEY_KP_4;
    keymap[GLFW_KEY_KP_5] = KEY_KP_5;
    keymap[GLFW_KEY_KP_6] = KEY_KP_6;
    keymap[GLFW_KEY_KP_7] = KEY_KP_7;
    keymap[GLFW_KEY_KP_8] = KEY_KP_8;
    keymap[GLFW_KEY_KP_9] = KEY_KP_9;
    keymap[GLFW_KEY_KP_DIVIDE] = KEY_KP_DIVIDE;  
    keymap[GLFW_KEY_KP_MULTIPLY] = KEY_KP_MULTIPLY;  
    keymap[GLFW_KEY_KP_SUBTRACT] = KEY_KP_SUBTRACT;  
    keymap[GLFW_KEY_KP_ADD] = KEY_KP_ADD;  
    keymap[GLFW_KEY_KP_DECIMAL] = KEY_KP_DECIMAL;  
    keymap[GLFW_KEY_KP_EQUAL] = KEY_KP_EQUAL;  
    keymap[GLFW_KEY_KP_ENTER] = KEY_KP_ENTER;
}

static void GLFWCALL keyCallback(int key, int action)
{
	assert(NULL != inputHandler);
	inputHandler(DEVICE_KEYBOARD, keymap[key], (INPUTSTATE)action);
}

static void GLFWCALL mouseButtonCallback(int button, int action)
{
	assert(NULL != inputHandler);
	inputHandler(DEVICE_MOUSE, MOUSE_BUTTON_BASE + button, (INPUTSTATE)action);
}

static void GLFWCALL mousePosCallback(int x, int y)
{
	assert(NULL != inputHandler);
	if (x - old_mousex)
	{
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABSX, x);
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_RELX, x - old_mousex);	
		old_mousex = x;
	}
		
	if (y - old_mousey)
	{
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABSY, y);
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_RELY, y - old_mousey);
		old_mousey = y;
	}
}

static void GLFWCALL mouseWheelCallback(int pos)
{
	assert(NULL != inputHandler);
	if (pos - old_wheel)
	{
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABSWHEEL, pos);
		inputHandler(DEVICE_MOUSE, MOUSE_AXIS_RELWHEEL, pos - old_wheel);
		old_wheel = pos;
	}
}

int Platform_init()
{
    NOTIFY("Initializing platform layer");
    if (!glfwInit())
	{
		return 0;
	}
	
    mapKeys();
	
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glfwEnable(GLFW_STICKY_KEYS);
	glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);	
	
	
	isInitialized = TRUE;
	return 1;
}

BOOL Platform_isInitialized()
{
	return isInitialized;
}

void Platform_terminate()
{
    NOTIFY("Terminating platform layer");
    glfwTerminate();
	isInitialized = FALSE;
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

void Platform_update()
{
	glfwPollEvents();

}

void Platform_registerInputHandler(void (*handler)(int device, int trigger, INPUTSTATE state))
{
	assert(NULL != handler);	

	// This must be set before setting callbacks
	inputHandler = handler;
	
	glfwSetMouseButtonCallback(&mouseButtonCallback);
	glfwSetMousePosCallback(&mousePosCallback);
	glfwSetMouseWheelCallback(&mouseWheelCallback);
	glfwSetKeyCallback(&keyCallback);
}


