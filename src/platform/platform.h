/**
 * @file platform.h
 * @brief Main file of platform module.
 * @if doc_files
 * @ingroup smug_platform
 * @endif
 */

/**
 * @defgroup smug_platform Platform
 * @{
 */

#ifndef SMUG_PLATFORM_PLATFORM_H
#define SMUG_PLATFORM_PLATFORM_H

#include <common/common.h>
#include <platform/threads.h>
#include <utils/vector_type.h>

// Common key identifiers
typedef float INPUTSTATE;
#define INPUTSTATE_PRESSED 1.0f
#define INPUTSTATE_RELEASED 0.0f


// All normal A-Z and 0-9 keys are indexed by their upper case ASCII character
#define KEY_SPACE        32
#define KEY_SPECIAL      256
#define KEY_ESC          (KEY_SPECIAL+1)
#define KEY_F1           (KEY_SPECIAL+2)
#define KEY_F2           (KEY_SPECIAL+3)
#define KEY_F3           (KEY_SPECIAL+4)
#define KEY_F4           (KEY_SPECIAL+5)
#define KEY_F5           (KEY_SPECIAL+6)
#define KEY_F6           (KEY_SPECIAL+7)
#define KEY_F7           (KEY_SPECIAL+8)
#define KEY_F8           (KEY_SPECIAL+9)
#define KEY_F9           (KEY_SPECIAL+10)
#define KEY_F10          (KEY_SPECIAL+11)
#define KEY_F11          (KEY_SPECIAL+12)
#define KEY_F12          (KEY_SPECIAL+13)
#define KEY_F13          (KEY_SPECIAL+14)
#define KEY_F14          (KEY_SPECIAL+15)
#define KEY_F15          (KEY_SPECIAL+16)
#define KEY_F16          (KEY_SPECIAL+17)
#define KEY_F17          (KEY_SPECIAL+18)
#define KEY_F18          (KEY_SPECIAL+19)
#define KEY_F19          (KEY_SPECIAL+20)
#define KEY_F20          (KEY_SPECIAL+21)
#define KEY_F21          (KEY_SPECIAL+22)
#define KEY_F22          (KEY_SPECIAL+23)
#define KEY_F23          (KEY_SPECIAL+24)
#define KEY_F24          (KEY_SPECIAL+25)
#define KEY_F25          (KEY_SPECIAL+26)
#define KEY_UP           (KEY_SPECIAL+27)
#define KEY_DOWN         (KEY_SPECIAL+28)
#define KEY_LEFT         (KEY_SPECIAL+29)
#define KEY_RIGHT        (KEY_SPECIAL+30)
#define KEY_LSHIFT       (KEY_SPECIAL+31)
#define KEY_RSHIFT       (KEY_SPECIAL+32)
#define KEY_LCTRL        (KEY_SPECIAL+33)
#define KEY_RCTRL        (KEY_SPECIAL+34)
#define KEY_LALT         (KEY_SPECIAL+35)
#define KEY_RALT         (KEY_SPECIAL+36)
#define KEY_TAB          (KEY_SPECIAL+37)
#define KEY_ENTER        (KEY_SPECIAL+38)
#define KEY_BACKSPACE    (KEY_SPECIAL+39)
#define KEY_INSERT       (KEY_SPECIAL+40)
#define KEY_DEL          (KEY_SPECIAL+41)
#define KEY_PAGEUP       (KEY_SPECIAL+42)
#define KEY_PAGEDOWN     (KEY_SPECIAL+43)
#define KEY_HOME         (KEY_SPECIAL+44)
#define KEY_END          (KEY_SPECIAL+45)
#define KEY_KP_0         (KEY_SPECIAL+46)
#define KEY_KP_1         (KEY_SPECIAL+47)
#define KEY_KP_2         (KEY_SPECIAL+48)
#define KEY_KP_3         (KEY_SPECIAL+49)
#define KEY_KP_4         (KEY_SPECIAL+50)
#define KEY_KP_5         (KEY_SPECIAL+51)
#define KEY_KP_6         (KEY_SPECIAL+52)
#define KEY_KP_7         (KEY_SPECIAL+53)
#define KEY_KP_8         (KEY_SPECIAL+54)
#define KEY_KP_9         (KEY_SPECIAL+55)
#define KEY_KP_DIVIDE    (KEY_SPECIAL+56)
#define KEY_KP_MULTIPLY  (KEY_SPECIAL+57)
#define KEY_KP_SUBTRACT  (KEY_SPECIAL+58)
#define KEY_KP_ADD       (KEY_SPECIAL+59)
#define KEY_KP_DECIMAL   (KEY_SPECIAL+60)
#define KEY_KP_EQUAL     (KEY_SPECIAL+61)
#define KEY_KP_ENTER     (KEY_SPECIAL+62)
#define KEY_LAST         KEY_KP_ENTER
#define KEY_BASE		 0
#define KEY_COUNT        KEY_LAST-KEY_BASE

// Mouse button definitions
#define MOUSE_BUTTON_BASE	0
#define MOUSE_BUTTON_1      MOUSE_BUTTON_BASE+0
#define MOUSE_BUTTON_2      MOUSE_BUTTON_BASE+1
#define MOUSE_BUTTON_3      MOUSE_BUTTON_BASE+2
#define MOUSE_BUTTON_4      MOUSE_BUTTON_BASE+3
#define MOUSE_BUTTON_5      MOUSE_BUTTON_BASE+4
#define MOUSE_BUTTON_6      MOUSE_BUTTON_BASE+5
#define MOUSE_BUTTON_7      MOUSE_BUTTON_BASE+6
#define MOUSE_BUTTON_8      MOUSE_BUTTON_BASE+7
#define MOUSE_BUTTON_9      MOUSE_BUTTON_BASE+8
#define MOUSE_BUTTON_10     MOUSE_BUTTON_BASE+9
#define MOUSE_BUTTON_11     MOUSE_BUTTON_BASE+10
#define MOUSE_BUTTON_12     MOUSE_BUTTON_BASE+11
#define MOUSE_BUTTON_LAST   MOUSE_BUTTON_12
#define MOUSE_BUTTON_COUNT  MOUSE_BUTTON_LAST-MOUSE_BUTTON_BASE

// Mouse button aliases
#define MOUSE_BUTTON_LEFT   MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT  MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_3

// Mouse axis definitions
#define MOUSE_AXIS_BASE MOUSE_BUTTON_LAST+1
#define MOUSE_AXIS_XPOS MOUSE_AXIS_BASE+0
#define MOUSE_AXIS_XNEG MOUSE_AXIS_BASE+1
#define MOUSE_AXIS_YPOS MOUSE_AXIS_BASE+2
#define MOUSE_AXIS_YNEG MOUSE_AXIS_BASE+3
#define MOUSE_AXIS_WHEELPOS MOUSE_AXIS_BASE+4
#define MOUSE_AXIS_WHEELNEG MOUSE_AXIS_BASE+5
#define MOUSE_AXIS_LAST MOUSE_AXIS_WHEELNEG
#define MOUSE_AXIS_COUNT MOUSE_AXIS_LAST-MOUSE_AXIS_BASE

#define MOUSE_BASE MOUSE_BUTTON_BASE
#define MOUSE_LAST MOUSE_AXIS_LAST
#define MOUSE_COUNT MOUSE_LAST-MOUSE_BASE

// Joystick button identifiers
#define JOYSTICK_BUTTON_BASE	   0
#define JOYSTICK_BUTTON_1          JOYSTICK_BUTTON_BASE+0
#define JOYSTICK_BUTTON_2          JOYSTICK_BUTTON_BASE+1
#define JOYSTICK_BUTTON_3          JOYSTICK_BUTTON_BASE+2
#define JOYSTICK_BUTTON_4          JOYSTICK_BUTTON_BASE+3
#define JOYSTICK_BUTTON_5          JOYSTICK_BUTTON_BASE+4
#define JOYSTICK_BUTTON_6          JOYSTICK_BUTTON_BASE+5
#define JOYSTICK_BUTTON_7          JOYSTICK_BUTTON_BASE+6
#define JOYSTICK_BUTTON_8          JOYSTICK_BUTTON_BASE+7
#define JOYSTICK_BUTTON_9          JOYSTICK_BUTTON_BASE+8
#define JOYSTICK_BUTTON_10         JOYSTICK_BUTTON_BASE+9
#define JOYSTICK_BUTTON_11         JOYSTICK_BUTTON_BASE+10
#define JOYSTICK_BUTTON_12         JOYSTICK_BUTTON_BASE+11
#define JOYSTICK_BUTTON_13         JOYSTICK_BUTTON_BASE+12
#define JOYSTICK_BUTTON_14         JOYSTICK_BUTTON_BASE+13
#define JOYSTICK_BUTTON_15         JOYSTICK_BUTTON_BASE+14
#define JOYSTICK_BUTTON_16         JOYSTICK_BUTTON_BASE+15
#define JOYSTICK_BUTTON_LAST       JOYSTICK_BUTTON_16
#define JOYSTICK_BUTTON_COUNT      JOYSTICK_BUTTON_LAST-JOYSTICK_BUTTON_BASE

// Joystick axis identifiers
#define JOYSTICK_AXIS_BASE JOYSTICK_BUTTON_LAST+1
#define JOYSTICK_AXIS_1_XPOS JOYSTICK_AXIS_BASE+0
#define JOYSTICK_AXIS_1_XNEG JOYSTICK_AXIS_BASE+1
#define JOYSTICK_AXIS_1_YPOS JOYSTICK_AXIS_BASE+2
#define JOYSTICK_AXIS_1_YNEG JOYSTICK_AXIS_BASE+3
#define JOYSTICK_AXIS_2_XPOS JOYSTICK_AXIS_BASE+4
#define JOYSTICK_AXIS_2_XNEG JOYSTICK_AXIS_BASE+5
#define JOYSTICK_AXIS_2_YPOS JOYSTICK_AXIS_BASE+6
#define JOYSTICK_AXIS_2_YNEG JOYSTICK_AXIS_BASE+7
#define JOYSTICK_AXIS_3_XPOS JOYSTICK_AXIS_BASE+8
#define JOYSTICK_AXIS_3_XNEG JOYSTICK_AXIS_BASE+9
#define JOYSTICK_AXIS_3_YPOS JOYSTICK_AXIS_BASE+10
#define JOYSTICK_AXIS_3_YNEG JOYSTICK_AXIS_BASE+11
#define JOYSTICK_AXIS_4_XPOS JOYSTICK_AXIS_BASE+12
#define JOYSTICK_AXIS_4_XNEG JOYSTICK_AXIS_BASE+13
#define JOYSTICK_AXIS_4_YPOS JOYSTICK_AXIS_BASE+14
#define JOYSTICK_AXIS_4_YNEG JOYSTICK_AXIS_BASE+15
#define JOYSTICK_AXIS_LAST JOYSTICK_AXIS_4_YNEG
#define JOYSTICK_AXIS_COUNT JOYSTICK_AXIS_LAST-JOYSTICK_AXIS_BASE

#define JOYSTICK_BASE JOYSTICK_BUTTON_BASE
#define JOYSTICK_LAST JOYSTICK_AXIS_BASE
#define JOYSTICK_COUNT JOYSTICK_LAST-JOYSTICK_BASE

//Device identifiers
#define DEVICE_KEYBOARD 0
#define DEVICE_MOUSE 1
#define DEVICE_JOYSTICK_BASE 2
#define DEVICE_JOYSTICK_1 DEVICE_JOYSTICK_BASE+2
#define DEVICE_JOYSTICK_2 DEVICE_JOYSTICK_BASE+1
#define DEVICE_JOYSTICK_3 DEVICE_JOYSTICK_BASE+2
#define DEVICE_JOYSTICK_4 DEVICE_JOYSTICK_BASE+3
#define DEVICE_JOYSTICK_5 DEVICE_JOYSTICK_BASE+4
#define DEVICE_JOYSTICK_6 DEVICE_JOYSTICK_BASE+5
#define DEVICE_JOYSTICK_7 DEVICE_JOYSTICK_BASE+6
#define DEVICE_JOYSTICK_8 DEVICE_JOYSTICK_BASE+7
#define DEVICE_JOYSTICK_9 DEVICE_JOYSTICK_BASE+8
#define DEVICE_JOYSTICK_10 DEVICE_JOYSTICK_BASE+9
#define DEVICE_JOYSTICK_11 DEVICE_JOYSTICK_BASE+10
#define DEVICE_JOYSTICK_12 DEVICE_JOYSTICK_BASE+11
#define DEVICE_JOYSTICK_13 DEVICE_JOYSTICK_BASE+12
#define DEVICE_JOYSTICK_14 DEVICE_JOYSTICK_BASE+13
#define DEVICE_JOYSTICK_15 DEVICE_JOYSTICK_BASE+14
#define DEVICE_JOYSTICK_16 DEVICE_JOYSTICK_BASE+15
#define DEVICE_JOYSTICK_LAST DEVICE_JOYSTICK_16
#define DEVICE_LAST DEVICE_JOYSTICK_LAST
#define DEVICE_BASE DEVICE_KEYBOARD
#define DEVICE_COUNT DEVICE_LAST-DEVICE_BASE

typedef enum
{
    SMUG_OPENED,
    SMUG_RESTORED,
    SMUG_ACTIVATED,
    SMUG_DEACTIVATED,
    SMUG_MINIMIZED,
    SMUG_CLOSED
} SMUG_WINDOW_STATE_CHANGE;

/**
 * Initializes the platform layer and opens a window.
 * @param width the width of the window in pixels
 * @param height the height of the window in pixels
 * @param fullscreen fullscreen or not
 * @return int 0 on fail, 1 on succes
 */
int Platform_init(int width, int height, BOOL fullscreen);

void Platform_initInput(void);

/**
 * Returns true if the      is initialized
 */
BOOL Platform_isInitialized(void);

/**
 * Terminate Platform
 */
void Platform_terminate(void);

/** Will be called by the platform OR the blocking version of the smug engine at the set fps
 *  interval.
 */
void Platform_internalHeartbeat(void);

/**
 * Check if the window is open
 */
BOOL Platform_isWindowOpen(void);


/**
 * Refresh graphhics in window
 */
void Platform_refreshWindow(void);

/**
 * Returns the current size of the window
 */
Vector Platform_getWindowSize(void);

void Platform_setWindowResizeCallback(void(*callback)(int, int));

void Platform_setWindowStateChangeCallback(void(*callback)(SMUG_WINDOW_STATE_CHANGE));

void Platform_setKillCallback(void(*callback)(void));

void Platform_setTouchEventCallback(void(*callback)(int, int, int));

void Platform_setLogicCallback(void (*callback)(void));

void Platform_enableLogicCallback(BOOL enable);

void Platform_setLogicFps(float fps);

/**
 * Get the current      time
 */
TIME Platform_getTime(void);

TIME Platform_getDiscreteTime(void);

void Platform_stepDiscreteTime(void);

float Platform_getInterpolationFactor(void);

/**
 * Sleep for a given amount of seconds
 */
void Platform_sleep(TIME seconds);

/**
 * Update platform layer
 */
void Platform_update(void);

/**
 * Register a handler for new input events
 */
void Platform_registerInputHandler(void (*handler)(int device, int trigger, INPUTSTATE state));

/**
 * Unregister previously registered handler
 */
void Platform_unregisterInputHandler(void);

/**
 * Get the input state of a given trigger
 */
INPUTSTATE Platform_getInputState(int device, int trigger);

/**
 * Search for connected joysticks
 */
void Platform_detectJoysticks(void);

#endif // SMUG_PLATFORM_PLATFORM_H

/**@}*/
