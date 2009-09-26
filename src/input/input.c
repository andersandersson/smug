#include "input.h"

#include <stdlib.h>
#include <stdio.h>

#include "utils/arraylist.h"
#include "utils/linkedlist.h"

#include "platform/platform.h"

static BOOL isInitialized = FALSE;

static ArrayList* controllers;

static void assertDevice(unsigned int device)
{
//	ArrayList* buttons = ArrayList_get(devices, device);
	//if (buttons == NULL)
	//{
	
	
	
//	}


}

static void inputHandler(int device, int trigger, INPUTSTATE state)
{
	fprintf(stderr, "Input: Got trigger: %i\n", trigger);
}


int Input_init()
{
	assert(Platform_isInitialized());
	assert(Platform_isWindowOpen());

	Platform_registerInputHandler(&inputHandler);
	
	controllers = ArrayList_new();
	
	isInitialized = TRUE;
	return 1;
}

BOOL Input_isInitialized()
{
	return isInitialized;
}

void Input_terminate()
{	
	isInitialized = FALSE;
}

void Input_connectController(Controller* controller, unsigned int slot)
{
	ArrayList_set(controllers, slot, controller);
	Controller_connect(controller, slot);
}

void Input_disconnectController(unsigned int slot)
{
	Controller* c = ArrayList_get(controllers, slot);
	if (NULL != c)
	{
		Controller_disconnect(c);
	}
	ArrayList_set(controllers, slot, NULL);	
}

BOOL Input_getKey(unsigned int key)
{
	assert(key >= KEY_BASE && key <= KEY_LAST);
	return Platform_getInputState(DEVICE_KEYBOARD, key);
}

Point Input_getMousePos()
{
    Vector windowSize = Platform_getWindowSize();
    return Point_createFromXY((Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_ABS_XPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_ABS_XNEG) * windowSize.d[0]),
                                    (Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_ABS_YPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_ABS_YNEG) * windowSize.d[1]));
}

INPUTSTATE Input_getInputState(int device, int id)
{
	return Platform_getInputState(device, id);	
}

BOOL Input_getMouseButton(unsigned int button)
{
	return Platform_getInputState(DEVICE_MOUSE, button) == INPUTSTATE_PRESSED;
}

