//int (*smugInit)() = Engine_init;

int smugInit()
{
    Engine_init();
}

/** 
 * Close down Engine
 */
void smugTerminate()
{
    Engine_terminate();
}

/** 
 * Run the engine, 
 */
void smugRun()
{
    Engine_run();
}
