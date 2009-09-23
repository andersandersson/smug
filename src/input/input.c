#include "input.h"

#include <stdlib.h>
#include <stdio.h>

#include "utils/arraylist.h"
#include "utils/linkedlist.h"

#include "platform/platform.h"

static BOOL isInitialized = FALSE;

static ArrayList* deviceArray;

static INPUTSTATE* keyboardState;
static INPUTSTATE* mouseState;
static INPUTSTATE* joystickState[DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE];



static void assertDevice(unsigned int device)
{
//	ArrayList* buttons = ArrayList_get(devices, device);
	//if (buttons == NULL)
	//{
	
	
	
//	}


}

static void inputHandler(int device, int trigger, INPUTSTATE state)
{
	static INPUTSTATE* array;
	array = ArrayList_get(deviceArray, device);
	array[trigger] = state;
	//fprintf(stderr, "Got trigger: %i\n", trigger);
}

static INPUTSTATE* newStateArray(unsigned int size)
{
	INPUTSTATE* arr  = malloc(sizeof(INPUTSTATE) * (size));
	int i;
	for (i = 0; i < size; i++)
	{
		arr[i] = INPUTSTATE_RELEASED;
	}
	return arr;
}

int Input_init()
{
	assert(Platform_isInitialized());
	assert(Platform_isWindowOpen());
	
	deviceArray = ArrayList_newFromCapacity(DEVICE_LAST + 1);
	keyboardState = newStateArray(KEY_LAST + 1);
	ArrayList_set(deviceArray, DEVICE_KEYBOARD, (void*)keyboardState);
	mouseState = newStateArray(MOUSE_LAST + 1);
	ArrayList_set(deviceArray, DEVICE_MOUSE, (void*)mouseState);
	int i;
	for (i = DEVICE_JOYSTICK_BASE; i < DEVICE_JOYSTICK_LAST; i++)
	{
		joystickState[i - DEVICE_JOYSTICK_BASE] = newStateArray(JOYSTICK_LAST + 1);
		ArrayList_set(deviceArray, i, (void*)joystickState[i - DEVICE_JOYSTICK_BASE]);
	}

	Platform_registerInputHandler(&inputHandler);
	isInitialized = TRUE;
	return 1;
}

BOOL Input_isInitialized()
{
	return isInitialized;
}

void Input_terminate()
{
	ArrayList_deleteContents(deviceArray, &free);
	ArrayList_delete(deviceArray);
	isInitialized = FALSE;
}



void Input_addKeyHook(unsigned int key, Hook* hook)
{
	//addHook(DEVICE_KEYBOARD, key, hook);

}

void Input_addMouseMoveHook(unsigned int axis, Hook* hook)
{
	//addHook(DEVICE_MOUSE, axis, hook);

}

void Input_addMouseButtonHook(unsigned int button, Hook* hook)
{
	//addHook(DEVICE_MOUSE, button, hook);	

}

INPUTSTATE Input_getKey(unsigned int key)
{
	assert(key >= KEY_BASE && key <= KEY_LAST);
	return keyboardState[key];
}

INPUTSTATE Input_getMouseXPos()
{
	return mouseState[MOUSE_AXIS_ABSX];
}

INPUTSTATE Input_getMouseYPos()
{
	return mouseState[MOUSE_AXIS_ABSY];
}

INPUTSTATE Input_getMouseWheelPos()
{
	return mouseState[MOUSE_AXIS_ABSWHEEL];	
}

INPUTSTATE Input_getMouseXRel()
{
	return mouseState[MOUSE_AXIS_RELX];
}

INPUTSTATE Input_getMouseYRel()
{
	return mouseState[MOUSE_AXIS_RELY];
}

INPUTSTATE Input_getMouseWheelRel()
{
	return mouseState[MOUSE_AXIS_RELWHEEL];	
}

INPUTSTATE Input_getMouseButton(unsigned int button)
{
	assert(button >= MOUSE_BUTTON_BASE && button <= MOUSE_BUTTON_LAST);
	return mouseState[button];
}

