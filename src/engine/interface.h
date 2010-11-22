/** @file interface.h
  * @brief API for engine module
  */

#ifndef SMUG_ENGINE_INTERFACE_H
#define SMUG_ENGINE_INTERFACE_H

/** Initialize Engine
  *
  * @return Returns zero on error, non-zero on success.
  */
int smugInit(void);

/**
  * Close down Engine
  */
void smugTerminate(void);

/**
  * Run the engine,
  */
void smugRun(void);

#endif //SMUG_ENGINE_INTERFACE_H
