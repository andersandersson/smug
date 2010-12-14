/** @file interface.h
  * @brief API for engine module
  */

#ifndef SMUG_ENGINE_INTERFACE_H
#define SMUG_ENGINE_INTERFACE_H

#ifdef INTERNAL_INCLUDE
#include <common/common.h>
#endif /* INTERNAL_INCLUDE */

/** Initialize Engine
  *
  * @return Returns zero on error, non-zero on success.
  */
SMUGEXPORT int smugInit(void);

/**
  * Close down Engine
  */
SMUGEXPORT void smugTerminate(void);

/**
  * Run the engine,
  */
/* See note in interface.c */
/* void smugRun(void); */

/* int smugAddObject(SmugObject* gameObject); */
/* SmugObject* smugRemoveObject(int index); */
/* void smugAddDrawableToObject(SmugObject* object, SmugDrawable* drawable); */
/* void smugAddBodyToObject(SmugObject* object, SmugBody* drawable); */
/* void smugSetObjectMovementModel(SmugObject* object, int movement); */
// const int SMUG_MOVEMENT_EXPLICIT = 0 /* User will update position */
// const int SMUG_MOVEMENT_PHYSICS  = 1 /* Laws of physics will apply */
// const int SMUG_MOVEMENT_PATH     = 2 /* Object will follow set path */

#endif //SMUG_ENGINE_INTERFACE_H
