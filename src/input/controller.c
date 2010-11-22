#include "controller.h"

#include "common/log.h"
#include "platform/platform.h"
#include "input/input.h"

#include "stdlib.h"

Button* Button_new(Controller* controller, unsigned int device, unsigned trigger)
{
    Button* b = (Button*)malloc(sizeof(Button));
    b->controller = controller;
    b->device = device;
    b->trigger = trigger;
    b->state = 0.0f;
    b->oldstate = 0.0f;
    b->triggered = FALSE;
    return b;
}

void Button_delete(void* button)
{
    free(button);
}

Axis* Axis_new(Controller* controller, unsigned int posdevice, unsigned postrigger, unsigned int negdevice, unsigned negtrigger)
{
    Axis* a = (Axis*)malloc(sizeof(Axis));
    a->controller = controller;
    a->posdevice = posdevice;
    a->postrigger = postrigger;
    a->negdevice = negdevice;
    a->negtrigger = negtrigger;
    a->state = 0.0f;
    a->oldstate = 0.0f;
    a->posstate = 0.0f;
    a->negstate = 0.0f;
    a->triggered = FALSE;
    return a;
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

// Callback used for setting the positive component of an axis
static int setAxisPosCallback(void* axis, void* state)
{
    Axis* a = (Axis*)axis;
    INPUTSTATE s = *((INPUTSTATE*)state);
    float threshold = a->controller->type->threshold;
    if (a->state < threshold && s > threshold)
    {
        a->triggered = TRUE;
    }
    a->posstate = s;
    a->state = a->posstate - a->negstate;
    return 1;
}

// Callback used for setting the negative component of an axis
static int setAxisNegCallback(void* axis, void* state)
{
    Axis* a = (Axis*)axis;
    INPUTSTATE s = *((INPUTSTATE*)state);
    float threshold = a->controller->type->threshold;
    if (-a->state < threshold && s > threshold)
    {
        a->triggered = TRUE;
    }
    a->negstate = s;
    a->state = a->posstate - a->negstate;
    return 1;
}

// Callback used for setting the state of a button
static int setButtonCallback(void* button, void* state)
{
    Button* b = (Button*)button;
    INPUTSTATE s = *((INPUTSTATE*)state);
    float threshold = b->controller->type->threshold;
    if (b->state < threshold && s > threshold)
    {
        b->triggered = TRUE;
    }
    b->state = s;
    return 1;
}

void Controller_bindButton(Controller* controller, unsigned int button, unsigned int device, unsigned int trigger)
{
    if (button >= controller->type->num_of_buttons)
    {
        WARNING("Invalid button id");
        return;
    }
    Button* b = Button_new(controller, device, trigger);
    ArrayList_set(controller->buttons, button, b);
    Input_setTriggerControllerHook(device, trigger, b, &setButtonCallback);
}

void Controller_bindAxis(Controller* controller, unsigned int axis, unsigned int device_neg, unsigned int trigger_neg,
																	unsigned int device_pos, unsigned int trigger_pos)
{
    if (axis >= controller->type->num_of_axes)
    {
        WARNING("Invalid axis id");
        return;
    }
    Axis* a = Axis_new(controller, device_neg, trigger_neg, device_pos, trigger_pos);
    ArrayList_set(controller->axes, axis, a);
    Input_setTriggerControllerHook(device_neg, trigger_neg, a, &setAxisNegCallback);
    Input_setTriggerControllerHook(device_pos, trigger_pos, a, &setAxisPosCallback);
}

INPUTSTATE Controller_getAxisValue(Controller* controller, unsigned int axis)
{
    Axis* theaxis = ArrayList_get(controller->axes, axis);
    if (NULL != theaxis)
    {
        return theaxis->state;
    }
    return 0.0f;
}

INPUTSTATE Controller_getButtonValue(Controller* controller, unsigned int button)
{
    Button* thebutton = ArrayList_get(controller->buttons, button);
    if (NULL != thebutton)
    {
        return thebutton->state;
    }
    return 0.0f;
}

BOOL Controller_wasAxisTriggered(Controller* controller, unsigned int axis)
{
    BOOL retval = FALSE;
    Button* theaxis = ArrayList_get(controller->axes, axis);
    if (NULL != theaxis)
    {
        retval = theaxis->triggered;
        theaxis->triggered = FALSE;
    }
    return retval;
}

BOOL Controller_wasButtonTriggered(Controller* controller, unsigned int button)
{
    BOOL retval = FALSE;
    Button* thebutton = ArrayList_get(controller->buttons, button);
    if (NULL != thebutton)
    {
        retval = thebutton->triggered;
        thebutton->triggered = FALSE;
    }
    return retval;
}

INPUTSTATE Controller_getAxisChange(Controller* controller, unsigned int axis)
{
    INPUTSTATE change = 0.0f;
    Axis* theaxis = ArrayList_get(controller->axes, axis);
    if (NULL != theaxis)
    {
        change = theaxis->state - theaxis->oldstate;
        theaxis->oldstate = theaxis->state;
    }
    return change;
}

INPUTSTATE Controller_getButtonChange(Controller* controller, unsigned int button)
{
    INPUTSTATE change = 0.0f;
    Button* thebutton = ArrayList_get(controller->buttons, button);
    if (NULL != thebutton)
    {
        change = thebutton->state - thebutton->oldstate;
        thebutton->oldstate = thebutton->state;
    }
    return change;
}
