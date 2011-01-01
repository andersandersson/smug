#ifndef SMUG_UTILS_INTERFACE_H
#define SMUG_UTILS_INTERFACE_H

#include <utils/rectangle_type.h>

typedef Rectangle SmugRect;

int smugPrintf(const char* format, ...);

SmugRect smugRect_Create(float x, float y, float w, float h);

#endif // SMUG_UTILS_INTERFACE_H
