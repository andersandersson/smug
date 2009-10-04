#include "input.h"

#include <stdlib.h>
#include <stdio.h>

#include "utils/arraylist.h"
#include "utils/linkedlist.h"
#include "utils/hook.h"

#include "graphics/graphics.h"
#include "platform/platform.h"

static BOOL isInitialized = FALSE;

static ArrayList* controllers = NULL;

static ArrayList* hooks_devices = NULL; // a 2d array with linked lists, holds [device][trigger][hooks]

static void initHookArray()
{
    hooks_devices = ArrayList_newFromCapacity(DEVICE_COUNT);
    int i;
    ArrayList* triggerList;
    triggerList = ArrayList_newFromCapacity(KEY_COUNT);
    ArrayList_set(hooks_devices, DEVICE_KEYBOARD - DEVICE_BASE, triggerList);    
    for (i = 0; i < KEY_COUNT; i++)
    {
        ArrayList_set(triggerList, i, LinkedList_new());
    }

    triggerList = ArrayList_newFromCapacity(MOUSE_COUNT);
    ArrayList_set(hooks_devices, DEVICE_MOUSE - DEVICE_BASE, triggerList);    
    for (i = 0; i < MOUSE_COUNT; i++)
    {
        ArrayList_set(triggerList, i, LinkedList_new());
    }

    int j;
    for (j = 0; j < JOYSTICK_COUNT; j++)
    {
        triggerList = ArrayList_newFromCapacity(JOYSTICK_COUNT);
        ArrayList_set(hooks_devices, DEVICE_JOYSTICK_BASE - DEVICE_BASE + j, triggerList);    
        for (i = 0; i < JOYSTICK_COUNT; i++)
        {
            ArrayList_set(triggerList, i, LinkedList_new());
        }
    }
}

static void freeHooksPart(void* hooklist)
{
    assert(NULL != hooklist);
    LinkedList* list = (LinkedList*)hooklist;
    LinkedList_deleteContents(list, &Hook_delete);
    LinkedList_delete(list);
}

static void freeTriggersPart(void* triggerarray)
{
    assert(NULL != triggerarray);
    ArrayList_deleteContents(triggerarray, &freeHooksPart); 
    ArrayList_delete(triggerarray);
}

static void freeHookArray()
{
    assert(NULL != hooks_devices);
    ArrayList_deleteContents(hooks_devices, &freeTriggersPart);
    ArrayList_delete(hooks_devices);
    hooks_devices = NULL;
}

static void setHook(unsigned int device, unsigned int trigger, void* data, int (*hook)(void*, void*))
{
    ArrayList* triggerarray = ArrayList_get(hooks_devices, device);
    LinkedList* hooklist = ArrayList_get(triggerarray, trigger);
    LinkedList_addLast(hooklist, Hook_newFromFunction(data, hook));
}

static void inputHandler(int device, int trigger, INPUTSTATE state)
{
    assert(NULL != hooks_devices);
	//fprintf(stderr, "Input: Got trigger: %i\n", trigger);
    
    // Call all  hooks connected to the trigger
    ArrayList* triggerarray = ArrayList_get(hooks_devices, device);
    LinkedList* hooklist = ArrayList_get(triggerarray, trigger);
    Hook* hook;
    Node* node = hooklist->first;
    while(NULL != node)
    {
        hook = node->item;
        Hook_call(hook, &state);
        node = node->next;
    }    
}

int Input_init()
{
	assert(Platform_isInitialized());
	assert(Platform_isWindowOpen());

	Platform_registerInputHandler(&inputHandler);
	
	controllers = ArrayList_new();
    initHookArray();
	
	isInitialized = TRUE;
	return 1;
}

BOOL Input_isInitialized()
{
	return isInitialized;
}

void Input_terminate()
{
    // This myst be done before freeing hook array
    Platform_unregisterInputHandler();
    
    freeHookArray();
	isInitialized = FALSE;
}

void Input_setTriggerControllerHook(unsigned int device, unsigned int trigger, void* object, int (*function)(void*, void*))
{
    assert(NULL != object);
    assert(NULL != function);
    setHook(device, trigger, object, function);
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

INPUTSTATE Input_getKey(unsigned int key)
{
	assert(key >= KEY_BASE && key <= KEY_LAST);
	return Platform_getInputState(DEVICE_KEYBOARD, key);
}

Point Input_getMousePos()
{
    Vector windowSize = Vector_multiply(Platform_getWindowSize(), 0.5);
    return Point_createFromXY((Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XNEG)) * windowSize.d[0],
                                    (Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YNEG)) * windowSize.d[1]);
}


INPUTSTATE Input_getMouseButton(unsigned int button)
{
	return Platform_getInputState(DEVICE_MOUSE, button) == INPUTSTATE_PRESSED;
}

Vector Input_getMouseScreenMovement()
{
    static Point last_pos;
    Point pos = Input_getMouseScreenPosition();
    Vector retval = Point_distanceToPoint(pos, last_pos);
    last_pos = pos;
    return retval;
}

Point Input_getMouseScreenPosition()
{
    Vector screenSize = Graphics_getScreenSize();
    return Point_createFromXY((Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XNEG)) * Vector_getX(&screenSize) * 0.5f,
                                    (Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YNEG)) * Vector_getY(&screenSize) * 0.5f);
}

Vector Input_getMouseMovement()
{
    static Point last_pos;
    Point pos = Input_getMousePosition();
    Vector retval = Point_distanceToPoint(pos, last_pos);
    last_pos = pos;
    return retval;
}

Point Input_getMousePosition()
{
    return Point_createFromXY(Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XNEG),
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YPOS) - 
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YNEG));
}




INPUTSTATE Input_getInputState(int device, int id)
{
	return Platform_getInputState(device, id);	
}

