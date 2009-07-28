#include "hooks.h"

#include <stdlib.h>

Hook* Hook_new()
{
    Hook* hook = malloc(sizeof(Hook));
    return hook;
}

void Hook_delete(Hook* hook)
{
    free(hook);
}

int Hook_call(Hook* hook, void* arg)
{
    return hook->function(hook->data, arg);
}
