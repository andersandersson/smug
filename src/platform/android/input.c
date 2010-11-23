#include <stdio.h>
#include <common/log.h>
#include <platform/platform.h>

// Holds information of physical joystick
typedef struct JoystickInfo
{
    int buttons;
    int axes;
    INPUTSTATE state[JOYSTICK_LAST - JOYSTICK_BASE];
    BOOL connected;
} JoystickInfo;

// State array for keyboard
static INPUTSTATE keyState[KEY_LAST - KEY_BASE];

// State array for mouse
static INPUTSTATE mouseState[MOUSE_LAST - MOUSE_BASE];

// State array for joysticks
static JoystickInfo joyState[DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE];

// Holds the current input handler
static void (*inputHandler)(int device, int trigger, INPUTSTATE state) = NULL;

// Converts a platform key id to a trigger id
static int convertKeyToTrigger(int key) { return KEY_BASE + key; }

// Converts a platform moouse button id to a trigger id
static int convertMouseButtonToTrigger(int button) { return MOUSE_BUTTON_BASE + button; }

// Reset the state of a joystick
static void clearJoystickState(unsigned int joystick)
{
    static int i;
    joyState[joystick].connected = FALSE;
    joyState[joystick].axes = 0;
    joyState[joystick].buttons = 0;
    for (i = 0; i < JOYSTICK_LAST - JOYSTICK_BASE; i++)
    {
        joyState[joystick].state[i] = 0.0f;
    }
}

// Reset the state of all input
static void clearInputState(void)
{
    static int i;
    for (i = 0; i < KEY_LAST - KEY_BASE; i++)
    {
        keyState[i] = 0.0f;
    }
    for (i = 0; i < MOUSE_LAST - MOUSE_BASE; i++)
    {
        mouseState[i] = 0.0f;
    }
    for (i = 0; i < DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE; i++)
    {
        clearJoystickState(i);
    }
}

// Callback for platform key events
/* static void GLFWCALL keyCallback(int key, int action)
{
    assert(NULL != inputHandler);
    static int trigger;
    trigger = convertKeyToTrigger(key);
    keyState[trigger] = (INPUTSTATE)action;
    inputHandler(DEVICE_KEYBOARD, trigger, keyState[trigger]);
} */

// Callback for platform mousebutton events
/* static void GLFWCALL mouseButtonCallback(int button, int action)
{
    assert(NULL != inputHandler);
    static int trigger;
    trigger = convertMouseButtonToTrigger(button);
    mouseState[trigger] = (INPUTSTATE)action;
    inputHandler(DEVICE_MOUSE, trigger, mouseState[trigger]);
} */

// Callback for platform mouse move events
/* static void GLFWCALL mousePosCallback(int x, int y)
{
    assert(NULL != inputHandler);
    static float absx;
    static float absy;
    static float abs_posx;
    static float abs_posy;
    static float abs_negx;
    static float abs_negy;

    // Calc centered position on window
    absx = ((float)x - windowSize.d[0]/2) / windowSize.d[0]*2;
    absy = ((float)y - windowSize.d[1]/2) / windowSize.d[1]*2;

    // Clamp position to range -1.0 to 1.0
    if (absx > 1.0f) absx = 1.0f;
    if (absx < -1.0f) absx = -1.0f;
    if (absy > 1.0f) absy = 1.0f;
    if (absy < -1.0f) absy = -1.0f;

    // Separate axes into pos/neg components
    if (absx >= 0.0f)
    {
        abs_posx = absx;
        abs_negx = 0.0f;
    }
    else
    {
        abs_posx = 0.0f;
        abs_negx = -absx;
    }
    if (absy >= 0.0f)
    {
        abs_posy = absy;
        abs_negy = 0.0f;
    }
    else
    {
        abs_posy = 0.0f;
        abs_negy = -absy;
    }

    // Send trigger events
    if (abs_posx - mouseState[MOUSE_AXIS_XPOS])
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_XPOS, abs_posx);
        mouseState[MOUSE_AXIS_XPOS] = abs_posx;
    }

    if (abs_posy - mouseState[MOUSE_AXIS_YPOS])
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_YPOS, abs_posy);
        mouseState[MOUSE_AXIS_YPOS] = abs_posy;
    }

    if (abs_negx - mouseState[MOUSE_AXIS_XNEG])
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_XNEG, abs_negx);
        mouseState[MOUSE_AXIS_XNEG] = abs_negx;
    }

    if (abs_negy - mouseState[MOUSE_AXIS_YNEG])
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_YNEG, abs_negy);
        mouseState[MOUSE_AXIS_YNEG] = abs_negy;
    }
} */

// Callback for platform mouse wheel events
/* static void GLFWCALL mouseWheelCallback(int pos)
{
    assert(NULL != inputHandler);
    static int old_pos = 0;

    // Handle mousewheel as two digital triggers.
    // Since we won't get an event for "trigger released" we have
    // to send both pressed and released to simulate a tapped trigger.
    if (pos - old_pos > 0)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_WHEELPOS, INPUTSTATE_PRESSED);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_WHEELPOS, INPUTSTATE_RELEASED);
    }
    else
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_WHEELNEG, INPUTSTATE_PRESSED);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_WHEELNEG, INPUTSTATE_RELEASED);
    }

    old_pos = pos;
} */

// Update the state of all connected joysticks
/* static void updateJoysticks(void)
{
    static unsigned char buttons[JOYSTICK_BUTTON_LAST - JOYSTICK_BUTTON_BASE];
    static float axes[JOYSTICK_BUTTON_LAST - JOYSTICK_BUTTON_BASE];

    int j;
    for (j = 0; j < DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE; j++)
    {
        if (joyState[j].connected)
        {
            glfwGetJoystickButtons(j, buttons, joyState[j].buttons);
            glfwGetJoystickPos(j, axes, joyState[j].axes);

            int i;
            for (i = 0; i < JOYSTICK_BUTTON_LAST - JOYSTICK_BUTTON_BASE; i++)
            {
                if (joyState[j].state[JOYSTICK_BUTTON_BASE + i] != buttons[i])
                {
                    inputHandler(DEVICE_JOYSTICK_BASE + j, JOYSTICK_BUTTON_BASE + i, buttons[i]);
                    joyState[j].state[JOYSTICK_BUTTON_BASE + i] = buttons[i];
                }
            }
            for (i = 0; i < JOYSTICK_AXIS_LAST - JOYSTICK_AXIS_BASE; i++)
            {
                if (joyState[j].state[JOYSTICK_AXIS_BASE + i] != axes[i])
                {
                    static float apos;
                    static float aneg;

                    // Separate axis into pos/neg components
                    if (axes[i] >= 0.0f)
                    {
                        apos = axes[i];
                        aneg = 0.0f;
                    }
                    else
                    {
                        apos = 0.0f;
                        aneg = -axes[i];
                    }

                    // Send trigger events
                    if (joyState[j].state[JOYSTICK_AXIS_BASE + i*2 + 0] != apos)
                    {
                        inputHandler(DEVICE_JOYSTICK_BASE + j, JOYSTICK_AXIS_BASE + i*2 + 0, apos);
                        joyState[j].state[JOYSTICK_AXIS_BASE + i*2 + 0] = apos;
                    }

                    if (joyState[j].state[JOYSTICK_AXIS_BASE + i*2 + 1] != aneg)
                    {
                        inputHandler(DEVICE_JOYSTICK_BASE + j, JOYSTICK_AXIS_BASE + i*2 + 1, aneg);
                        joyState[j].state[JOYSTICK_AXIS_BASE + i*2 + 1] = aneg;
                    }
                }
            }
        }
    }
} */

void Platform_initInput()
{
    // Initialize device state arrays
    clearInputState();

    // This must be set before setting callbacks
    Platform_unregisterInputHandler();

    // Set callbacks for input
    // glfwSetMouseButtonCallback(&mouseButtonCallback);
    // glfwSetMousePosCallback(&mousePosCallback);
    // glfwSetMouseWheelCallback(&mouseWheelCallback);
    // glfwSetKeyCallback(&keyCallback);

    //Platform_detectJoysticks();
}

void Platform_detectJoysticks(void)
{
    // noop for now
/*     int j;
    for (j = 0; j < DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE; j++)
    {
        joyState[j].connected = (BOOL) glfwGetJoystickParam(j, GLFW_PRESENT);
        if (joyState[j].connected)
        {
            joyState[j].buttons = glfwGetJoystickParam(j, GLFW_BUTTONS);
            joyState[j].axes = glfwGetJoystickParam(j, GLFW_AXES);
        }
        else
        {
            clearJoystickState(j);
        }
    } */
}

// A placeholder for an inputhandler
static void dummyInputHandler(int device, int trigger, INPUTSTATE state)
{
    fprintf(stderr, "Got input event. Device: %i, trigger: %i, state: %f\n", device, trigger, state);
}

void Platform_update(void)
{
    // noop
/*     glfwPollEvents();

    updateJoysticks(); */
}

void Platform_registerInputHandler(void (*handler)(int device, int trigger, INPUTSTATE state))
{
    assert(NULL != handler);

    inputHandler = handler;
}

void Platform_unregisterInputHandler(void)
{
    Platform_registerInputHandler(&dummyInputHandler);
}

INPUTSTATE Platform_getInputState(int device, int trigger)
{
    assert(device >= DEVICE_BASE && device <= DEVICE_LAST);
    switch (device)
    {
        case DEVICE_KEYBOARD:
        {
            assert(trigger >= KEY_BASE && trigger <= KEY_LAST);
            return keyState[trigger];
            break;
        }
        case DEVICE_MOUSE:
        {
            assert(trigger >= MOUSE_BASE && trigger <= MOUSE_LAST);
            return mouseState[trigger];
            break;
        }
        default:
        {
            assert(trigger >= JOYSTICK_BASE && trigger <= JOYSTICK_LAST);
            return joyState[device - DEVICE_JOYSTICK_BASE].state[trigger];
            break;
        }
    }
}
