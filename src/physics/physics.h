#ifndef PHYSICS_H
#define PHYSICS_H

#include "body.h"
#include "utils/hook.h"

int Physics_init();
void Physics_terminate();

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook);
void Physics_addBody(Body* body);
void Physics_removeBody(Body* body);
void Physics_update(TIME time);

#endif
