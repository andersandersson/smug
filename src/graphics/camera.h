#ifndef SMUG_GRAPHICS_CAMERA_H
#define SMUG_GRAPHICS_CAMERA_H

#include "common/common.h"

#include "utils/point.h"
#include "utils/vector.h"

typedef struct Camera
{
    Point pos;
    float rot;
    Vector scale;
    Vector center;
} Camera;


Camera* Camera_new(Vector center);
void Camera_delete(void* camera);

void Camera_setPosition(Camera* camera, Point p);
void Camera_setRotation(Camera* camera, float r);
void Camera_setZoom(Camera* camera, float zoom);


#endif //SMUG_GRAPHICS_CAMERA_H
