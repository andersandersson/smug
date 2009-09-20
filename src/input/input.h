#ifndef SMUG_INPUT_H
#define SMUG_INPUT_H

#include "common/common.h"
#include "utils/hook.h"
#include "platform/platform.h"





/**
 * Initialize Input subsystem
 */
int Input_init(int width, int height);

/** Terminate the input component of the engine.
  */
void Input_terminate();


void Input_addKeyHook(unsigned int key, Hook* hook)
void Input_addMouseMoveHook(Hook* hook)
void Input_addMouseButtonHook(unsigned int key, Hook* hook)

#endif //SMUG_INPUT_H
