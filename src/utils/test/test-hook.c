#include <stdio.h>
#include <assert.h>

#include "hook.h"

int my_func(void* x, void* y)
{
    return 2*(int)x+(int)y;
}

int main()
{
    Hook* hook = Hook_new();

    hook->data = (void*) 12;
    hook->function = my_func;

    assert(29 == Hook_call(hook, (void*)5));

    Hook_delete(hook);
    return 0;
}
