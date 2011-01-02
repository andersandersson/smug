#ifndef SMUG_PLATFORM_INTERFACE_H
#define SMUG_PLATFORM_INTERFACE_H

/* SmugCapabilities smugPlatform_detectCapabilities(); */
/* typedef struct SmugCapabilities
{
    BOOL hasTouch;
    BOOL hasPointer;
    BOOL hasKeyboard;
    BOOL canSetWindowSize;
} SmugCapabilities; */

SMUGEXPORT void smugSetLogicCallback(void (*callback)(void));

SMUGEXPORT double smugGetTime(void);

SMUGEXPORT void smugSetLogicFps(float fps);

#endif // SMUG_PLATFORM_INTERFACE_H
