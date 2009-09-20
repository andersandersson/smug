#ifndef PLATFORM_H
#define PLATFORM_H

#include "threads.h"
#include "common/common.h"

// All normal A-Z and 0-9 keys are indexed by their upper case ASCII character
#define KEY_UNKNOWN      -1
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

// Mouse button definitions
#define MOUSE_BUTTON_1      0
#define MOUSE_BUTTON_2      1
#define MOUSE_BUTTON_3      2
#define MOUSE_BUTTON_4      3
#define MOUSE_BUTTON_5      4
#define MOUSE_BUTTON_6      5
#define MOUSE_BUTTON_7      6
#define MOUSE_BUTTON_8      7
#define MOUSE_BUTTON_LAST   MOUSE_BUTTON_8

// Mouse button aliases
#define MOUSE_BUTTON_LEFT   MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT  MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_3


// Joystick identifiers
#define JOYSTICK_BUTTON_1          0
#define JOYSTICK_BUTTON_2          1
#define JOYSTICK_BUTTON_3          2
#define JOYSTICK_BUTTON_4          3
#define JOYSTICK_BUTTON_5          4
#define JOYSTICK_BUTTON_6          5
#define JOYSTICK_BUTTON_7          6
#define JOYSTICK_BUTTON_8          7
#define JOYSTICK_BUTTON_9          8
#define JOYSTICK_BUTTON_10         9
#define JOYSTICK_BUTTON_11         10
#define JOYSTICK_BUTTON_12         11
#define JOYSTICK_BUTTON_13         12
#define JOYSTICK_BUTTON_14         13
#define JOYSTICK_BUTTON_15         14
#define JOYSTICK_BUTTON_16         15
#define JOYSTICK_BUTTON_LAST       JOYSTICK_BUTTON_16


//Device identifiers
#define DEVICE_KEYBOARD 0
#define DEVICE_MOUSE 1
#define DEVICE_JOYSTICK_1 2
#define DEVICE_JOYSTICK_2 DEVICE_JOYSTICK_1+1
#define DEVICE_JOYSTICK_3 DEVICE_JOYSTICK_1+2
#define DEVICE_JOYSTICK_4 DEVICE_JOYSTICK_1+3
#define DEVICE_JOYSTICK_5 DEVICE_JOYSTICK_1+4
#define DEVICE_JOYSTICK_6 DEVICE_JOYSTICK_1+5
#define DEVICE_JOYSTICK_7 DEVICE_JOYSTICK_1+6
#define DEVICE_JOYSTICK_8 DEVICE_JOYSTICK_1+7
#define DEVICE_JOYSTICK_9 DEVICE_JOYSTICK_1+8
#define DEVICE_JOYSTICK_10 DEVICE_JOYSTICK_1+9
#define DEVICE_JOYSTICK_11 DEVICE_JOYSTICK_1+10
#define DEVICE_JOYSTICK_12 DEVICE_JOYSTICK_1+11
#define DEVICE_JOYSTICK_13 DEVICE_JOYSTICK_1+12
#define DEVICE_JOYSTICK_14 DEVICE_JOYSTICK_1+13
#define DEVICE_JOYSTICK_15 DEVICE_JOYSTICK_1+14
#define DEVICE_JOYSTICK_16 DEVICE_JOYSTICK_1+15
#define DEVICE_JOYSTICK_FIRST DEVICE_JOYSTICK_1
#define DEVICE_JOYSTICK_LAST DEVICE_JOYSTICK_16
#define DEVICE_LAST DEVICE_JOYSTICK_LAST


int Platform_init();
void Platform_terminate();

BOOL Platform_openWindow(int width, int height, BOOL fullscreen);
void Platform_closeWindow();
BOOL Platform_isWindowOpen();
void Platform_refreshWindow();
TIME Platform_getTime();
void Platform_sleep(TIME seconds);
BOOL Platform_getKey(int key);

#endif // PLATFORM_H
