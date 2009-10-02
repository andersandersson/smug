#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include "common/common.h"

#include "utils/arraylist.h"
#include "controllertype.h"

#include "platform/platform.h"

typedef struct Controller
{
	BOOL _connected;
	unsigned int _slot;
    ControllerType* type;

    ArrayList* buttons;
    ArrayList* axes;

} Controller;


Controller* Controller_new(ControllerType* type);
void Controller_delete(void* controller);

void Controller_connect(Controller* controller, unsigned int slot);
void Controller_disconnect(Controller* controller);

void Controller_bindButton(Controller* controller, unsigned int button, unsigned int device, unsigned int trigger);
void Controller_bindAxis(Controller* controller, unsigned int axis, unsigned int device_neg, unsigned int trigger_neg, 
																	unsigned int device_pos, unsigned int trigger_pos);
INPUTSTATE Controller_getAxisValue(Controller* controller, unsigned int axis);
INPUTSTATE Controller_getButtonValue(Controller* controller, unsigned int button);
BOOL Controller_wasAxisTriggered(Controller* controller, unsigned int axis);
BOOL Controller_wasButtonTriggered(Controller* controller, unsigned int button);
INPUTSTATE Controller_getAxisChange(Controller* controller, unsigned int axis);
INPUTSTATE Controller_getButtonChange(Controller* controller, unsigned int button);

typedef struct Button
{
    Controller* controller;
	unsigned int device;
	unsigned int trigger;
	
    INPUTSTATE state;
    INPUTSTATE oldstate;
    BOOL triggered;
} Button;
 
Button* Button_new(Controller* controller, unsigned int device, unsigned trigger);
void Button_delete(void* button);

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

Axis* Axis_new(Controller* controller, unsigned int posdevice, unsigned postrigger, unsigned int negdevice, unsigned negtrigger);
void Axis_delete(void* axis);



#endif // SMUG_INPUT_CONTROLLER_H
