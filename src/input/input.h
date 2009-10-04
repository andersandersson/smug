#ifndef SMUG_INPUT_H
#define SMUG_INPUT_H

#include "common/common.h"
#include "utils/hook.h"
#include "utils/point.h"
#include "platform/platform.h"

#include "controller.h"



/**
 * Initialize Input subsystem
 */
int Input_init();

/**
 * Return true if the system is initialized
 */
BOOL Input_isInitialized(); 

/** Terminate the input component of the engine.
  */
void Input_terminate();

INPUTSTATE Input_getKey(unsigned int key);
INPUTSTATE Input_getMouseButton(unsigned int button);
Vector Input_getMouseScreenMovement();
Point Input_getMouseScreenPosition();
Vector Input_getMouseMovement();
Point Input_getMousePosition();

INPUTSTATE Input_getInputState(int device, int id);

void Input_connectController(Controller* controller, unsigned int slot);
void Input_disconnectController(unsigned int slot);

void Input_setTriggerControllerHook(unsigned int device, unsigned int trigger, void* object, int (*function)(void*, void*));


#endif //SMUG_INPUT_H
