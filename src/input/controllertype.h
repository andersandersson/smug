#ifndef SMUG_CONTROLLERTYPE_H
#define SMUG_CONTROLLERTYPE_H


typedef struct ControllerType
{
    unsigned int id;
    unsigned int num_of_buttons;
    unsigned int num_of_axes;
    float threashold;
} ControllerType;

ControllerType* ControllerType_new(unsigned int buttons, unsigned int axes);




#endif // SMUG_CONTROLLERTYPE_H
