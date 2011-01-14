#include <smugstd.h>

#include <utils/arraylist.h>
#include <utils/linkedlist.h>
#include <utils/point.h>
#include <utils/hook.h>
#include <utils/vector.h>
#include <graphics/graphics.h>
#include <platform/platform.h>

#include <input/input.h>

// System init value
static BOOL isInitialized = FALSE;

// List of connected controllers
static ArrayList* controllers = NULL;

// A 2d table with linked lists, holds [device][trigger][list of hooks]
static ArrayList* hooks_devices = NULL;

// Initialize the array that holds all trigger hooks
static void initHookArray(void)
{
    // Dimension 1 of devices
    hooks_devices = ArrayList_newFromCapacity(DEVICE_COUNT);
    int i;
    ArrayList* triggerList;

    // Setup trigger lists for keyboard
    triggerList = ArrayList_newFromCapacity(KEY_COUNT);
    ArrayList_set(hooks_devices, DEVICE_KEYBOARD - DEVICE_BASE, triggerList);
    for (i = 0; i < KEY_COUNT; i++)
    {
        ArrayList_set(triggerList, i, LinkedList_new());
    }

    // Setup trigger lists for mouse
    triggerList = ArrayList_newFromCapacity(MOUSE_COUNT);
    ArrayList_set(hooks_devices, DEVICE_MOUSE - DEVICE_BASE, triggerList);
    for (i = 0; i < MOUSE_COUNT; i++)
    {
        ArrayList_set(triggerList, i, LinkedList_new());
    }

    // Setup trigger lists for joystick
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

// A helper function for freeing trigger hook table
static void freeHooksPart(void* hooklist)
{
    smug_assert(NULL != hooklist);
    LinkedList* list = (LinkedList*)hooklist;
    LinkedList_deleteContents(list, &Hook_delete);
    LinkedList_delete(list);
}

// A helper function for freeing trigger hook table
static void freeTriggersPart(void* triggerarray)
{
    smug_assert(NULL != triggerarray);
    ArrayList_deleteContents(triggerarray, &freeHooksPart);
    ArrayList_delete(triggerarray);
}

// Free the hook table
static void freeHookArray(void)
{
    smug_assert(NULL != hooks_devices);
    ArrayList_deleteContents(hooks_devices, &freeTriggersPart);
    ArrayList_delete(hooks_devices);
    hooks_devices = NULL;
}

// Connect a hook to a specific input trigger
static void setHook(unsigned int device, unsigned int trigger, void* data, int (*hook)(void*, void*))
{
    ArrayList* triggerarray = ArrayList_get(hooks_devices, device);
    LinkedList* hooklist = ArrayList_get(triggerarray, trigger);
    LinkedList_addLast(hooklist, Hook_newFromFunction(data, hook));
}

// Handle incoming input events
static void inputHandler(int device, int trigger, INPUTSTATE state)
{
    smug_assert(NULL != hooks_devices);
	//fprintf(stderr, "Input: Got trigger: %i\n", trigger);

    // Call all hooks connected to the trigger
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

int Input_init(void)
{
    smug_assert(!isInitialized);
	smug_assert(Platform_isInitialized());
	smug_assert(Platform_isWindowOpen());

    // Register this system as a handler for incoming events
	Platform_registerInputHandler(&inputHandler);

	controllers = ArrayList_new();
    initHookArray();

	isInitialized = TRUE;
	return 1;
}

BOOL Input_isInitialized(void)
{
	return isInitialized;
}

void Input_terminate(void)
{
    smug_assert(isInitialized);
    // This myst be done before freeing hook array
    Platform_unregisterInputHandler();

    freeHookArray();
	isInitialized = FALSE;
}

void Input_setTriggerControllerHook(unsigned int device, unsigned int trigger, void* object, int (*function)(void*, void*))
{
    smug_assert(NULL != object);
    smug_assert(NULL != function);
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
	smug_assert(key >= KEY_BASE && key <= KEY_LAST);
	return Platform_getInputState(DEVICE_KEYBOARD, key);
}

Point Input_getMousePos(void)
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

Vector Input_getMouseScreenMovement(void)
{
    static Point last_pos;
    Point pos = Input_getMouseScreenPosition();
    Vector retval = Point_distanceToPoint(pos, last_pos);
    last_pos = pos;
    return retval;
}

Point Input_getMouseScreenPosition(void)
{
    Vector screenSize = Platform_getWindowSize();
    return Point_createFromXY((Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XPOS) -
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_XNEG)) * Vector_getX(&screenSize) * 0.5f,
                                    (Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YPOS) -
                                    Platform_getInputState(DEVICE_MOUSE, MOUSE_AXIS_YNEG)) * Vector_getY(&screenSize) * 0.5f);
}

Vector Input_getMouseMovement(void)
{
    static Point last_pos;
    Point pos = Input_getMousePosition();
    Vector retval = Point_distanceToPoint(pos, last_pos);
    last_pos = pos;
    return retval;
}

Point Input_getMousePosition(void)
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

