#include "controllertype.h"

#include "stdlib.h"

ControllerType* ControllerType_new(unsigned int buttons, unsigned int axes)
{
    ControllerType* t = (ControllerType*)malloc(sizeof(ControllerType));
    t->threashold = 0.2;
    t->num_of_buttons = buttons;
    t->num_of_axes = axes;

    return t;
}





