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

Point Input_getMousePos();

BOOL Input_getKey(unsigned int key);
INPUTSTATE Input_getInputState(int device, int id);
BOOL Input_getMouseButton(unsigned int button);

void Input_connectController(Controller* controller, unsigned int slot);
void Input_disconnectController(unsigned int slot);



#endif //SMUG_INPUT_H
