#ifndef SMUG_ENGINE_INTERFACE_H
#define SMUG_ENGINE_INTERFACE_H

/** 
 * Initialize Engine
 */
int smugInit();

/** 
 * Close down Engine
 */
void smugTerminate();

/** 
 * Run the engine, 
 */
void smugRun();

#endif //SMUG_ENGINE_INTERFACE_H
