#include "controller.h"

#include "common/log.h"
#include "platform/platform.h"

#include "stdlib.h"

typedef struct Button
{
	unsigned int device;
	unsigned int trigger;
	
    INPUTSTATE state;
	BOOL triggered;		
} Button;
 
Button* Button_new(unsigned int device, unsigned trigger)
{
    Button* b = (Button*)malloc(sizeof(Button));
    b->device = device;
    b->trigger = trigger;
    b->state = 0.0f;
    b->triggered = FALSE;
    return b;
}

void Button_delete(void* button)
{
    free(button);
}

typedef struct Axis
{
	unsigned int posdevice;
    unsigned int negdevice;
	unsigned int postrigger;
	unsigned int negtrigger;
    
    INPUTSTATE state;
	BOOL triggered;		
} Axis;

Axis* Axis_new(unsigned int posdevice, unsigned postrigger, unsigned int negdevice, unsigned negtrigger)
{
    Axis* b = (Axis*)malloc(sizeof(Axis));
    b->posdevice = posdevice;
    b->postrigger = postrigger;
    b->negdevice = negdevice;
    b->negtrigger = negtrigger;
    b->state = 0.0f;
    b->triggered = FALSE;
    return b;
}

void Axis_delete(void* axis)
{
    free(axis);
}

Controller* Controller_new(ControllerType* type)
{
    assert(NULL != type);
	Controller* c = (Controller*)malloc(sizeof(Controller));
	
	c->_slot = 0;
	c->_connected = FALSE;
	c->type = type;
    
    c->buttons = ArrayList_newFromCapacity(type->num_of_buttons);
    c->axes = ArrayList_newFromCapacity(type->num_of_axes);
    
	return c;
}

void Controller_delete(void* controller)
{
	assert(NULL != controller);
	Controller* c = (Controller*)controller;
    
    ArrayList_deleteContents(c->buttons, &Button_delete);
    ArrayList_deleteContents(c->axes, &Axis_delete);
    ArrayList_delete(c->buttons);
    ArrayList_delete(c->axes);
	free(c);
}

void Controller_connect(Controller* controller, unsigned int slot)
{
	controller->_slot = slot;
	controller->_connected = TRUE;
}

void Controller_disconnect(Controller* controller)
{
	controller->_connected = FALSE;
}

void Controller_bindButton(Controller* controller, unsigned int button, unsigned int device, unsigned int trigger)
{
    if (button >= controller->type->num_of_buttons) 
    {
        WARNING("Invalid button id");
        return;
    }
    ArrayList_set(controller->buttons, button, Button_new(device, trigger));
}

void Controller_bindAxis(Controller* controller, unsigned int axis, unsigned int device_neg, unsigned int trigger_neg, 
																	unsigned int device_pos, unsigned int trigger_pos)
{
    if (axis >= controller->type->num_of_axes) 
    {
        WARNING("Invalid axis id");
        return;
    }
    ArrayList_set(controller->axes, axis, Axis_new(device_neg, trigger_neg, device_pos, trigger_pos));
}


