/** @file camera.h
  * @brief Defines the Camera type.
  * @ingroup smug_graphics
  */

/** @addtogroup smug_graphics
  * @{
  */

#ifndef SMUG_GRAPHICS_CAMERA_H
#define SMUG_GRAPHICS_CAMERA_H

#include "common/common.h"

#include "utils/point.h"
#include "utils/vector.h"

/**
 * A versatile camera for viewing the world.
 */
typedef struct Camera
{
    Point pos;
    float rot;
    Vector scale;
    Vector center;
} Camera;

/**
 * Create new Camera
 */
Camera* Camera_new(Vector center);

/**
 * Delete Camera
 */
void Camera_delete(void* camera);

/**
 * Set position of a Camera
 */
void Camera_setPosition(Camera* camera, Point p);

/**
 * Set rotation angle of a Camera
 */
void Camera_setRotation(Camera* camera, float r);

/**
 * Set zoom-level of a Camera
 */
void Camera_setZoom(Camera* camera, float zoom);


#endif //SMUG_GRAPHICS_CAMERA_H

/**@}*/
