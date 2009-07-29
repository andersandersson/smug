#include "threads.h"
#include "test-utils/assert.h"


int main()
{
    glfwInit();

    Mutex* mutex = Mutex_new();

    assertTrue(NULL != mutex->_mutex);

    Mutex_delete(mutex);

    glfwTerminate();
    return 0;
}
