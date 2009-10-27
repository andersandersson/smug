/** @file engine.h
  * @brief Header file for engine module
  * @ingroup smug_engine
  */

/** @defgroup smug_engine Engine
  * The Engine module is the central module of Smug. It contains the game loop and the World and GameObject types.
  * 
  * @{
  */
  
#ifndef SMUG_ENGINE_ENGINE_H
#define SMUG_ENGINE_ENGINE_H

#include "common/common.h"
#include "gameobject.h"

/** 
  * Initialize Engine
  *
  * After this function is run, the engine is considered 
  * up and running, even if the actual game loop has not 
  * started yet. This state, before the gameloop has started, 
  * should be used to load resources and set up scenes.
  * 
  * @return Returns zero on error, non-zero on success.
  */
int Engine_init();

/** 
  * Checks if the engine is initialized.
  *
  */
BOOL Engine_isInitialized();

/** 
  * Close down Engine
  *
  */
void Engine_terminate();

/** 
  * Run the engine.
  *
  * @pre The engine must be initialized.
  */
void Engine_run();

/** 
  *
  */
void Engine_addObject(GameObject* newObj);

#endif // SMUG_ENGINE_ENGINE_H

/**@}*/
