/**
 * @file blocking_engine.h
 * @brief Header file for a blocking version of the engine module
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @addtogroup smug_engine
 * @{
 */

#ifndef SMUG_ENGINE_BLOCKING_ENGINE_H
#define SMUG_ENGINE_BLOCKING_ENGINE_H

/**
 * Run the engine.
 *
 * @pre The engine must be initialized.
 */
void Engine_run(void);

void Engine_setLogicFps(float fps);

float Engine_getInterpolationFactor(void);

#endif // SMUG_ENGINE_BLOCKING_ENGINE_H

/**@}*/
