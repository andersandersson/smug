#include "rectangle.h"

Rectangle Rectangle_create(double x, double y, double w, double h)
{
	Rectangle r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}