/**
 * @file engine.h
 * @brief Header file for engine module
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @defgroup smug_engine Engine
 * The Engine module is the central module of Smug. It contains the game loop and the World and GameObject types.
 *
 * @{
 */

#ifndef SMUG_ENGINE_ENGINE_H
#define SMUG_ENGINE_ENGINE_H

#include <common/common.h>
#include <engine/gameobject_type.h>

/**
 * Initialize Engine
 *
 * After this function is run, the engine is considered
 * up and running, even if the actual game loop has not
 * started yet. This state, before the gameloop has started,
 * should be used to load resources and set up scenes.
 *
 * Initializes logging, platform, signals, input, graphics and physics.
 *
 * @return Returns zero on error, non-zero on success.
 */
int Engine_init(BOOL verbose, BOOL console);

/**
 * Checks if the engine is initialized.
 */
BOOL Engine_isInitialized(void);

/**
 * Close down Engine
 */
void Engine_terminate(void);

/**
 *
 */
void Engine_addObject(GameObject* newObj);

void Engine_removeObject(GameObject* obj);

/**
 * Called by blocking engine OR platform once after each logic heartbeat.
 */
void Engine_commitPositionChanges(void);

#endif // SMUG_ENGINE_ENGINE_H

/**@}*/
