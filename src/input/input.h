#ifndef SMUG_INPUT_H
#define SMUG_INPUT_H

#include "common/common.h"
#include "utils/hook.h"
#include "platform/platform.h"





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
INPUTSTATE Input_getMouseXPos();
INPUTSTATE Input_getMouseYPos();
INPUTSTATE Input_getMouseWheelPos();
INPUTSTATE Input_getMouseXRel();
INPUTSTATE Input_getMouseYRel();
INPUTSTATE Input_getMouseWheelRel();
INPUTSTATE Input_getMouseButton(unsigned int button);

void Input_addKeyHook(unsigned int key, Hook* hook);
void Input_addMouseMoveHook(unsigned int axis, Hook* hook);
void Input_addMouseButtonHook(unsigned int button, Hook* hook);

#endif //SMUG_INPUT_H
