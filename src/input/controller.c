#include "controller.h"


Controller* Controller_new()
{
	Controller* c = (Controller*)malloc(sizeof(Controller));
	
	c->_slot = 0;
	c->_connected = FALSE;
	
	return c;
}

void Controller_delete(void* controller)
{
	assert(NULL != controller);
	
	free(controller);
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

