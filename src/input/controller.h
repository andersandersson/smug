#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

typedef struct Trigger
{
	unsigned int device;
	unsigned int trigger;
	
	BOOL triggered;
		
} Trigger;


typedef struct Controller
{
	BOOL _connected;
	unsigned int _slot;

} Controller;



Controller* Controller_new(unsigned int triggers, unsigned int axes);
void Controller_delete(void* controller);

void Controller_connect(Controller* controller, unsigned int slot);
void Controller_disconnect(Controller* controller);

void Controller_setTrigger(Controller* controller, unsigned int trigger, unsigned int device, unsigned int trigger);
void Controller_setAxis(Controller* controller, unsigned int axis, unsigned int device_neg, unsigned int trigger_neg, 
																	unsigned int device_pos, unsigned int trigger_pos);









#endif // SMUG_INPUT_CONTROLLER_H
