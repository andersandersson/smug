#include "camera.h"

#include "stdlib.h"

Camera* Camera_new(Vector center)
{
    Camera* ret = (Camera*)malloc(sizeof(Camera));
    ret->pos = Point_createFromXY(0.0f,0.0f);
    ret->scale = Vector_create2d(1.0f, 1.0f);
    ret->rot = 0;
    ret->center = center;

    return ret;
}
void Camera_delete(void* camera)
{   
    assert(NULL != camera);
    Camera* thecamera = (Camera*)camera;
    free(thecamera);
}

void Camera_setPosition(Camera* camera, Point p)
{
    assert(NULL != camera);
    
    camera->pos = p;
}
void Camera_setRotation(Camera* camera, float r)
{
    assert(NULL != camera);
    
    camera->rot = r;
}


void Camera_setZoom(Camera* camera, float zoom)
{
    assert(NULL != camera);

    if (zoom <= 0.01)
        zoom = 0.01;
    if (zoom >= 100)
       zoom = 99;
        
    camera->scale = Vector_create2d(zoom, zoom);
    
}

