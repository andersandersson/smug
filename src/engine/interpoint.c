#include <smugstd.h>

#include <common/common.h>
#include <platform/platform.h>
#include <utils/point.h>
#include <utils/vector_type.h>
#include <common/log.h>

#include <engine/interpoint.h>

Interpoint* Interpoint_new(Point p)
{
    Interpoint* newPoint = (Interpoint*)malloc(sizeof(Interpoint));
    newPoint->point = p;
    newPoint->oldPoint = p;
    newPoint->pointCommitted = TRUE;
    newPoint->pointChangedThisHeartbeat = FALSE;
    return newPoint;
}

void Interpoint_delete(Interpoint* self)
{
    free(self);
}

Point Interpoint_getPoint(Interpoint* self)
{
    // smug_printf("Getting interpoint: %f, %f", self.point.v.d[0], self.point.v.d[1]);
    return self->point;
}

void Interpoint_setTo(Interpoint* self, Point p)
{
    // smug_printf("Setting interpoint %i to %f, %f", self, Point_getX(p), Point_getY(p));
    self->point = p;
    self->oldPoint = p;
    self->pointChangedThisHeartbeat = TRUE;
    self->pointCommitted = FALSE;
    // smug_printf("Set interpoint to %f, %f", Point_getX(self.point), Point_getY(self.point));
}

void Interpoint_moveTo(Interpoint* self, Point p)
{
    // smug_printf("Moving interpoint %i to %f, %f", self, Point_getX(p), Point_getY(p));
    self->pointChangedThisHeartbeat = TRUE;
    if (self->pointCommitted)
    {
        self->oldPoint = self->point;
        self->pointCommitted = FALSE;
    }
    self->point = p;
}

void Interpoint_commit(Interpoint* self)
{
    if (!self->pointChangedThisHeartbeat)
    {
        self->oldPoint = self->point;
    }
    self->pointChangedThisHeartbeat = FALSE;
    self->pointCommitted = TRUE;
}

Point Interpoint_getInterpolated(Interpoint* self)
{
    static float factor;
    if (Point_equal(self->point, self->oldPoint))
    {
        // smug_printf("Points equal. Returning %i, %i", Point_getX(self->point), Point_getY(self->point));
        return self->point;
    }
    else
    {
        factor = Platform_getInterpolationFactor();
        if (factor > 1.0f)
        {
            WARNING("Interpolation factor larger than 1.0!");
        }
        Point ret = Point_linearInterpolate(self->oldPoint, self->point, min(factor, 1.0f));
        // smug_printf("Points not equal. Returning %i, %i", Point_getX(ret), Point_getY(ret));
        return ret;
    }
}
