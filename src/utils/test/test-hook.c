#include <stdio.h>
#include "test-utils/assert.h"

#include "hook.h"

int my_func(void* x, void* y)
{
    return 2*(int)x+(int)y;
}

int returned_true = 0;
int return_true(void* x, void* y)
{
    returned_true++;
    return TRUE;
}

int returned_false = 0;
int return_false(void* x, void* y)
{
    returned_false++;
    return FALSE;
}

int main()
{
    Hook* hook = Hook_new();
    Hook* true_1 = Hook_newFromFunction((void*)1, return_true);
    Hook* true_2 = Hook_newFromFunction((void*)2, return_true);
    Hook* false_1 = Hook_newFromFunction((void*)3, return_false);
    Hook* false_2 = Hook_newFromFunction((void*)4, return_false);
    LinkedList* list = LinkedList_new();
    

    hook->data = (void*) 12;
    hook->function = my_func;
    assert(29 == Hook_call(hook, (void*)5));

    LinkedList_addLast(list, true_1);
    LinkedList_addLast(list, true_2);
    LinkedList_addLast(list, false_1);
    LinkedList_addLast(list, false_2);

    assert(0 == returned_false);
    assert(0 == returned_true);
    assert(FALSE == Hook_callAll(list, (void*)9));
    assert(2 == returned_false);
    assert(2 == returned_true);
    assert(FALSE == Hook_callAllWhile(list, (void*)9, TRUE));
    assert(3 == returned_false);
    assert(4 == returned_true);
    assert(FALSE == Hook_callAllWhileNot(list, (void*)9, FALSE));
    assert(4 == returned_false);
    assert(6 == returned_true);

    LinkedList_delete(list);
    Hook_delete(true_1);
    Hook_delete(true_2);
    Hook_delete(false_1);
    Hook_delete(false_2);
    Hook_delete(hook);
    return 0;
}
