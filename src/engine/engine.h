#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"

/** 
 * Initialize Engine
 *
 * After this function is run, the engine is considered 
 * up and running, even if the actual game loop has not 
 * started yet. This state, before the gameloop has started, 
 * should be used to load resources and set up scenes. 
 */
int Engine_init();

/** 
 * Close down Engine
 */
void Engine_terminate();

/** 
 * Run the engine, 
 */
void Engine_run();


#endif
