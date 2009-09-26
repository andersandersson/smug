#include "platform.h"

#include "GL/glfw.h"
#include "common/log.h"
#include "stdlib.h"

#include "stdio.h"

typedef struct JoystickInfo
{
    int buttons;
    int axes;
    INPUTSTATE state[JOYSTICK_LAST - JOYSTICK_BASE];
    BOOL connected;
} JoystickInfo;

static BOOL isInitialized = FALSE;

static Vector windowSize;

static INPUTSTATE keyState[KEY_LAST - KEY_BASE];
static INPUTSTATE mouseState[MOUSE_LAST - MOUSE_BASE];
static JoystickInfo joyState[DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE];

static void (*inputHandler)(int device, int trigger, INPUTSTATE state) = NULL;

static int convertKeyToTrigger(int key) { return KEY_BASE + key; }
static int convertMouseButtonToTrigger(int button) { return MOUSE_BUTTON_BASE + button; }

static void clearJoystickState(unsigned int joystick)
{
    static int i;
    static int j;
    joyState[j].connected = FALSE;
    joyState[j].axes = 0;
    joyState[j].buttons = 0;
    for (i = 0; i < JOYSTICK_LAST - JOYSTICK_BASE; i++)
    {
        for (j = 0; j < DEVICE_JOYSTICK_LAST - DEVICE_JOYSTICK_BASE; j++)
        {
            joyState[i].state[j] = 0.0f;
        }
    }
}

static void clearInputState()
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


static void GLFWCALL keyCallback(int key, int action)
{
    assert(NULL != inputHandler);
    static int trigger;
    trigger = convertKeyToTrigger(key);
    keyState[trigger] = (INPUTSTATE)action;
    inputHandler(DEVICE_KEYBOARD, trigger, keyState[trigger]);
}

static void GLFWCALL mouseButtonCallback(int button, int action)
{
    assert(NULL != inputHandler);
    static int trigger;
    trigger = convertMouseButtonToTrigger(button);
    mouseState[trigger] = (INPUTSTATE)action;
    inputHandler(DEVICE_MOUSE, trigger, mouseState[trigger]);
}

static void GLFWCALL mousePosCallback(int x, int y)
{
    assert(NULL != inputHandler);
    static float absx;
    static float absy;
    static float abs_posx;
    static float abs_posy;
    static float abs_negx;
    static float abs_negy;
    static float rel_posx;
    static float rel_posy;
    static float rel_negx;
    static float rel_negy;

    // Calc centered position on window
    absx = ((float)x - windowSize.d[0]/2) / windowSize.d[0];
    absy = ((float)y - windowSize.d[1]/2) / windowSize.d[1];

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

    // Calculate relative movement
    rel_posx = abs_posx - mouseState[MOUSE_AXIS_ABS_XPOS];
    rel_posy = abs_posy - mouseState[MOUSE_AXIS_ABS_YPOS];
    rel_negx = abs_negx - mouseState[MOUSE_AXIS_ABS_XNEG];
    rel_negy = abs_negy - mouseState[MOUSE_AXIS_ABS_YNEG];

    // Send trigger events
    if (rel_posx)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_XPOS, abs_posx);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_XPOS, rel_posx);
        mouseState[MOUSE_AXIS_ABS_XPOS] = abs_posx;
        mouseState[MOUSE_AXIS_REL_XPOS] = rel_posx;
    }
    else
    {
        if (rel_posx - mouseState[MOUSE_AXIS_REL_XPOS])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_XPOS, 0.0f);
        }
    }

    if (rel_posy)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_YPOS, abs_posy);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_YPOS, rel_posy);
        mouseState[MOUSE_AXIS_ABS_YPOS] = abs_posy;
        mouseState[MOUSE_AXIS_REL_YPOS] = rel_posy;
    }
    else
    {
        if (rel_posy - mouseState[MOUSE_AXIS_REL_YPOS])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_YPOS, 0.0f);
        }
    }

    if (rel_negx)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_XNEG, abs_negx);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_XNEG, rel_negx);
        mouseState[MOUSE_AXIS_ABS_XNEG] = abs_negx;
        mouseState[MOUSE_AXIS_REL_XNEG] = rel_negx;
    }
    else
    {
        if (rel_negx - mouseState[MOUSE_AXIS_REL_XNEG])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_XNEG, 0.0f);
        }
    }

    if (rel_negy)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_YNEG, abs_negy);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_YNEG, rel_negy);
        mouseState[MOUSE_AXIS_ABS_YNEG] = abs_negy;
        mouseState[MOUSE_AXIS_REL_YNEG] = rel_negy;
    }
    else
    {
        if (rel_negy - mouseState[MOUSE_AXIS_REL_YNEG])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_YNEG, 0.0f);
        }
    }
}

static void GLFWCALL mouseWheelCallback(int pos)
{
    assert(NULL != inputHandler);
    static float absw;
    static float abs_posw;
    static float abs_negw;
    static float rel_posw;
    static float rel_negw;

    // Calc centered position on window
    absw = pos;

    // Clamp position to range -1.0 to 1.0
    if (absw > 1.0f) absw = 1.0f;
    if (absw < -1.0f) absw = -1.0f;
    if (absw > 1.0f) absw = 1.0f;
    if (absw < -1.0f) absw = -1.0f;

    // Separate axes into pos/neg components
    if (absw >= 0.0f)
    {
        abs_posw = absw;
        abs_negw = 0.0f;
    }
    else
    {
        abs_posw = 0.0f;
        abs_negw = -absw;
    }
    
    // Calculate relative movement
    rel_posw = abs_posw - mouseState[MOUSE_AXIS_ABS_WHEELPOS];
    rel_negw = abs_negw - mouseState[MOUSE_AXIS_ABS_WHEELNEG];

    // Send trigger events
    if (rel_posw)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_WHEELPOS, abs_posw);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_WHEELPOS, rel_posw);
        mouseState[MOUSE_AXIS_ABS_WHEELPOS] = abs_posw;
        mouseState[MOUSE_AXIS_REL_WHEELPOS] = rel_posw;
    }
    else
    {
        if (rel_posw - mouseState[MOUSE_AXIS_REL_WHEELPOS])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_WHEELPOS, 0.0f);
        }
    }

    if (rel_negw)
    {
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_ABS_WHEELNEG, abs_negw);
        inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_WHEELNEG, rel_negw);
        mouseState[MOUSE_AXIS_ABS_WHEELNEG] = abs_negw;
        mouseState[MOUSE_AXIS_REL_WHEELNEG] = rel_negw;
    }
    else
    {
        if (rel_negw - mouseState[MOUSE_AXIS_REL_WHEELNEG])
        {
            inputHandler(DEVICE_MOUSE, MOUSE_AXIS_REL_WHEELNEG, 0.0f);
        }
    }
}

static void updateJoystick()
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
}

static void detectJoysticks()
{
    int j;
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
    }
}

void dummyInputHandler(int device, int trigger, INPUTSTATE state)
{
    fprintf(stderr, "Got input event. Device: %i, trigger: %i, state: %f\n", device, trigger, state);
}

int Platform_init()
{
    NOTIFY("Initializing platform layer");
    if (!glfwInit())
    {
        return 0;
    }

    glfwDisable(GLFW_AUTO_POLL_EVENTS);
    glfwEnable(GLFW_STICKY_KEYS);
    glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);

    // Initialize device state arrays
    clearInputState();

    // This must be set before setting callbacks
    Platform_registerInputHandler(&dummyInputHandler);

    // Set callbacks for input
    glfwSetMouseButtonCallback(&mouseButtonCallback);
    glfwSetMousePosCallback(&mousePosCallback);
    glfwSetMouseWheelCallback(&mouseWheelCallback);
    glfwSetKeyCallback(&keyCallback);


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
    windowSize = Vector_create2d(width, height);
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

Vector Platform_getWindowSize()
{
    return windowSize;
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

    inputHandler = handler;
}

INPUTSTATE Platform_getInputState(int device, int trigger)
{
    assert(device > DEVICE_FIRST && device < DEVICE_LAST);
    switch (device)
    {
        case DEVICE_KEYBOARD:
        {
            assert(KEY_BASE <= trigger && KEY_LAST <= trigger);
            return keyState[trigger];
            break;
        }
        case DEVICE_MOUSE:
        {
            assert(MOUSE_BASE <= trigger && MOUSE_LAST <= trigger);
            return mouseState[trigger];
            break;
        }
        default:
        {
            assert(JOYSTICK_BASE <= trigger && JOYSTICK_LAST <= trigger);
            return joyState[device - DEVICE_JOYSTICK_BASE].state[trigger];
            break;
        }
    }
}

