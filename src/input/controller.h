#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include "common/common.h"

#include "utils/arraylist.h"
#include "controllertype.h"

#include "platform/platform.h"

/*
 * Controller
 * A virtual controller modelled with n, m analouge buttons and axes.
 * Should be looked at as a physical controller that
 * one connects to the input system. This controller should serve
 * as the top level interface to the player in order to hide
 * platform-specific input.
 */
typedef struct Controller
{
	BOOL _connected;
	unsigned int _slot;
    ControllerType* type;

    ArrayList* buttons;
    ArrayList* axes;

} Controller;

/**
 * Create new controller
 * @param ControllerType the type of controller instantiate
 * @return Controller
 */
Controller* Controller_new(ControllerType* type);

/**
 * Delete controller
 * @param controller
 */
void Controller_delete(void* controller);

/**
 * Connect controller to slot
 * @param controller
 * @param slot
 */
void Controller_connect(Controller* controller, unsigned int slot);

/**
 * Disconnect controller
 * @param controller
 */
void Controller_disconnect(Controller* controller);

/**
 * Bind a virtual button to a specific input trigger
 * @param controller
 * @param button id for which button to bind
 * @param device device id
 * @param trigger trigger id
 */
void Controller_bindButton(Controller* controller, unsigned int button, unsigned int device, unsigned int trigger);

/**
 * Bind a virtual axis to a specifi input trigger
 * @param controller
 * @param axis id for which axis to bind
 * @param device_neg device id for the trigger for the negative component
 * @param device_pos device id for the trigger for the negative component
 * @param trigger_pos device id for the trigger for the positive component
 * @param trigger_pos trigger id for the trigger for the positive component
 */
void Controller_bindAxis(Controller* controller, unsigned int axis, unsigned int device_neg, unsigned int trigger_neg,
																	unsigned int device_pos, unsigned int trigger_pos);

/**
 * Get the value of a virtual axis
 * @param controller
 * @param axis axis id
 * @return INPUTSTATE
 */
INPUTSTATE Controller_getAxisValue(Controller* controller, unsigned int axis);

/**
 * Get the value of a virtual button
 * @param controller
 * @param button button id
 * @return INPUTSTATE
 */
INPUTSTATE Controller_getButtonValue(Controller* controller, unsigned int button);

/**
 * Check if the axis was triggered since the last time this function was called.
 * @param controller
 * @param axis axis id
 * @return BOOL true if the axis was triggered
 */
BOOL Controller_wasAxisTriggered(Controller* controller, unsigned int axis);

/**
 * Check if the button was triggered since the last time this function was called.
 * @param controller
 * @param button button id
 * @return BOOL true if the button was triggered
 */
BOOL Controller_wasButtonTriggered(Controller* controller, unsigned int button);

/**
 * Get the relative change of the axis since the last time this funciton was called.
 * @param controller
 * @param axis axis id
 * @return INPUTSTATE relative change
 */
INPUTSTATE Controller_getAxisChange(Controller* controller, unsigned int axis);

/**
 * Get the relative chage of the axis since the last time this funcion was called.
 * @param controller
 * @param button button id
 * @return INPUTSTATE relative change
 */
INPUTSTATE Controller_getButtonChange(Controller* controller, unsigned int button);

/**
 * Button
 * Used internally in a controller for representing a virtual button.
 */
typedef struct Button
{
    Controller* controller;
	unsigned int device;
	unsigned int trigger;

    INPUTSTATE state;
    INPUTSTATE oldstate;
    BOOL triggered;
} Button;

/**
 * Creates a new button
 * @return Button
 */
Button* Button_new(Controller* controller, unsigned int device, unsigned trigger);

/**
 * Deletes button
 */
void Button_delete(void* button);

/**
 * Axis
 * used internally by controller for representing a virtual axis.
 */
typedef struct Axis
{
    Controller* controller;
	unsigned int posdevice;
    unsigned int negdevice;
	unsigned int postrigger;
	unsigned int negtrigger;

    INPUTSTATE posstate;
    INPUTSTATE negstate;
    INPUTSTATE state;
    INPUTSTATE oldstate;
    BOOL triggered;
} Axis;

/**
 * Create new axis
 * @return Axis
 */
Axis* Axis_new(Controller* controller, unsigned int posdevice, unsigned postrigger, unsigned int negdevice, unsigned negtrigger);

/**
 * Delete axis
 */
void Axis_delete(void* axis);



#endif // SMUG_INPUT_CONTROLLER_H
