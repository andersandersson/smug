#include <smugstd.h>

#include <utils/hook.h>

Hook* Hook_new(void)
{
    Hook* hook = malloc(sizeof(Hook));
    return hook;
}

Hook* Hook_newFromFunction(void* data, int (*function)(void*, void*))
{
    Hook* hook = Hook_new();

    hook->data = data;
    hook->function = function;

    return hook;
}

void Hook_delete(void* hook)
{
    free(hook);
}

int Hook_call(Hook* hook, void* arg)
{
    return hook->function(hook->data, arg);
}

int Hook_callAll(LinkedList* list, void* arg)
{
    Node* node = NULL;
    int result = 0;

    for(node = list->first; node != NULL; node = node->next)
    {
        result = ((Hook*) node->item)->function(((Hook*) node->item)->data, arg);
    }

    return result;
}

int Hook_callAllWhile(LinkedList* list, void* arg, int condition)
{
    Node* node = NULL;
    int result = 0;

    for(node = list->first; node != NULL; node = node->next)
    {
        result = ((Hook*) node->item)->function(((Hook*) node->item)->data, arg);

        if(result != condition)
        {
            return result;
        }
    }

    return result;
}


int Hook_callAllWhileNot(LinkedList* list, void* arg, int condition)
{
    Node* node = NULL;
    int result = 0;

    for(node = list->first; node != NULL; node = node->next)
    {
        result = ((Hook*) node->item)->function(((Hook*) node->item)->data, arg);

        if(result == condition)
        {
            return result;
        }
    }

    return result;
}
