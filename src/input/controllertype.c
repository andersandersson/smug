#include <smugstd.h>

#include <common/common.h>

#include <input/controllertype.h>

ControllerType* ControllerType_new(unsigned int buttons, unsigned int axes)
{
    ControllerType* t = (ControllerType*)malloc(sizeof(ControllerType));
    t->threshold = 0.4;
    t->num_of_buttons = buttons;
    t->num_of_axes = axes;

    return t;
}





