#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

void deleteInt(void* i)
{
    free(i);
}


int main()
{
    fprintf(stderr, "Testing array list.\n");
    ArrayList* list1 = ArrayList_new();
    ArrayList* list2 = ArrayList_newFromCapacity(100);
    
    
    fprintf(stderr, "Setting up lists.\n");
    int i;
    for (i = 0; i < 100; i++)
    {
        int* p = (int*)malloc(sizeof(int));
        *p = i;
        ArrayList_set(list1, i, p);
        ArrayList_set(list2, i, p);
    }
    
    fprintf(stderr, "List1 capacity: %i, List2 capacity: %i.\n", ArrayList_getCapacity(list1), ArrayList_getCapacity(list2));

    fprintf(stderr, "Comparing lists.\n");
    for (i = 0; i < 100; i++)
    {
        int* p1 = (int*)ArrayList_get(list1, i);
        int* p2 = (int*)ArrayList_get(list2, i);      
        
        if (NULL == p1 || NULL == p2)
        {
            fprintf(stderr, "Error, found null-pointers in list at element %i.\n", i);
            return 1;
        }
        
        if (*p1 != *p2)
        {
            fprintf(stderr, "Error, lists did not match at element %i.\n", i);
            return 1;     
        }
    } 


    fprintf(stderr, "Deleting contents.\n");
    ArrayList_deleteContents(list1, deleteInt);
    ArrayList_deleteContents(list2, deleteInt); 
    
    fprintf(stderr, "Deleting lists.\n");
    ArrayList_delete(list1);
    ArrayList_delete(list2);


    fprintf(stderr, "Done.\n");

    return 0;
}   