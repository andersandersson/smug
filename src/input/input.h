#ifndef SMUG_INPUT_H
#define SMUG_INPUT_H

#include "common/common.h"
#include "utils/hook.h"
#include "utils/point.h"
#include "platform/platform.h"

#include "controller.h"

/**
 * Initialize Input subsystem
 * @pre The platform layer must be initialized via Platform_init(...)
 */
int Input_init();

/**
 * Return true if the system is initialized
 */
BOOL Input_isInitialized();

/** 
 * Terminate the input component of the engine.
 * @pre the Input module must be initialized via Input_init(...)
 */
void Input_terminate();

/**
 * Get the state of a keyboard key, returns 
 * either INPUTSTATE_RELEASED or INPUTSTATE_PRESSED
 * @param key which key
 * @return INPUTSTATE 
 */
INPUTSTATE Input_getKey(unsigned int key);

/**
 * Get the state of a mouse button, returns 
 * either INPUTSTATE_RELEASED or INPUTSTATE_PRESSED
 * @param button which mousebutton
 * @return INPUTSTATE
 */
INPUTSTATE Input_getMouseButton(unsigned int button);

/**
 * Get the relative screen movement of the mouse in screen coordinates. 
 * The position is centered with origo in the center of the screen. 
 * @return Vector with relative values.
 */
Vector Input_getMouseScreenMovement();

/**
 * Get the screen position of the mouse. The position
 * is centered with origo in the center of the screen. 
 * @return Point with absoulte values.
 */
Point Input_getMouseScreenPosition();

/**
 * Get the relative movement of the mouse as an INPUTSTATE. 
 * This will treat the mouse as a generic axis.
 * The position is centered with origo in the center of the screen. 
 * @return Vector of INPUTSTATE with relative values.
 */
Vector Input_getMouseMovement();

/**
 * Get the position of the mouse as an INPUTSTATE. 
 * This will treat the mouse as a generic axis. The position
 * is centered with origo in the center of the screen. 
 * @return Point of INPUTSTATE with absoulte values.
 */
Point Input_getMousePosition();

/**
 * Get the inputstate of a given device  and trigger id.
 * @param device device id
 * @param id trigger id
 * @return INPUSTATE with current trigger state
 */
INPUTSTATE Input_getInputState(int device, int id);

/**
 * Connect a controller to the input system.
 * A connected controller will start recieving events from input.
 * If another controller already exists at the given slot it will
 * be disconnected.
 * @param controller the controller
 * @param slot which slot to connect to
 */
void Input_connectController(Controller* controller, unsigned int slot);

/**
 * Disconnect a connected controller from the input system.
 * The controller will stop recieving input events.
 * @param slot which controller to disconnect
 */
void Input_disconnectController(unsigned int slot);

/**
 * Set a listener for an input trigger.
 * @param device device id
 * @param trigger trigger id
 * @param object data that should be sent to the hook when calling
 * @param function listener
 */
void Input_setTriggerControllerHook(unsigned int device, unsigned int trigger, void* object, int (*function)(void*, void*));


#endif //SMUG_INPUT_H
