#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include "common/common.h"

#include "utils/arraylist.h"
#include "controllertype.h"

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









#endif // SMUG_INPUT_CONTROLLER_H
