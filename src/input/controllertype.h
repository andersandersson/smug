#ifndef SMUG_INPUT_CONTROLLERTYPE_H
#define SMUG_INPUT_CONTROLLERTYPE_H

/**
 * ControllerType
 * Defines a template for a controller.
 */
typedef struct ControllerType
{
    unsigned int id;
    unsigned int num_of_buttons;
    unsigned int num_of_axes;
    float threshold;
} ControllerType;

/**
 * Create a new ControllerType. All buttons and axes are 0-indexed
 * in the instantiated controller.
 * @param buttons how many buttons the controller will have
 * @param axes how many axes the controller will have
 */
ControllerType* ControllerType_new(unsigned int buttons, unsigned int axes);


#endif // SMUG_INPUT_CONTROLLERTYPE_H
