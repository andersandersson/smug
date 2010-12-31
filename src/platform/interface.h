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

void smugSetLogicCallback(void (*callback)(void));

double smugGetTime(void);

#endif // SMUG_PLATFORM_INTERFACE_H
